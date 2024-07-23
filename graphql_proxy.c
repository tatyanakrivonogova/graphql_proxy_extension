#include "io_uring/event_handling.h"
#include "http/input_parsing.h"
#include "schema/schema_converting.h"
#include "libgraphqlparser/schema_to_json.h"

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "executor/executor.h"
#include "postmaster/bgworker.h"
#include "miscadmin.h"
#include "postmaster/interrupt.h"
#include "tcop/tcopprot.h"

#include <arpa/inet.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <liburing.h>

#include "executor/spi.h"

#include <libpq/libpq.h>
#include <libpq/libpq-fs.h>
#include <postgresql/libpq-fe.h>


#define DEFAULT_PORT            (7879)
#define DEFAULT_BACKLOG_SIZE    (512)
#define MAX_ENTRIES          (10)

PG_MODULE_MAGIC;
static void graphql_proxy_start_worker(void);
PGDLLEXPORT void graphql_proxy_main(Datum main_arg);
int create_connection(PGconn** conn, char* conn_info);
void close_connection(PGconn** conn);
int exec_query(PGconn** conn, char *query, PGresult** res);
void test_connect(void);

void
_PG_init(void) {
	graphql_proxy_start_worker();
}

static void
graphql_proxy_start_worker(void) {
	BackgroundWorker worker;

	MemSet(&worker, 0, sizeof(BackgroundWorker));
	worker.bgw_flags = BGWORKER_SHMEM_ACCESS;
	worker.bgw_start_time = BgWorkerStart_PostmasterStart;
	strcpy(worker.bgw_library_name, "graphql_proxy");
	strcpy(worker.bgw_function_name, "graphql_proxy_main");
	strcpy(worker.bgw_name, "graphql_proxy graphql_proxy_server");
	strcpy(worker.bgw_type, "graphql_proxy graphql_proxy_server");

	RegisterBackgroundWorker(&worker);
}

void
graphql_proxy_main(Datum main_arg) {
    struct io_uring_params params;
    struct io_uring ring;
    int cqe_count;
    int listen_socket;
    const int val = 1;

    struct sockaddr_in sockaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(DEFAULT_PORT),
        .sin_addr.s_addr = INADDR_ANY,
    };
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // get json schema
    const char *json_schema = schema_to_json();
    // parse schema
    schema_convert(json_schema);


    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == -1) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("socket(): %s\n", errorbuf));
        return;
    }

    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    if (bind(listen_socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("bind() error: %s\n", errorbuf));
        goto socket_bind_fail;
    }

    if (listen(listen_socket, DEFAULT_BACKLOG_SIZE)) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("listen() error: %s\n", errorbuf));
        goto socket_listen_fail;
    }

    memset(&params, 0, sizeof(params));

    if (io_uring_queue_init_params(MAX_ENTRIES, &ring, &params)) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("uring_init_params() error: %s\n", errorbuf));
    }

    if (!(params.features & IORING_FEAT_FAST_POLL)) {
        elog(LOG, "IORING_FEAT_FAST_POLL is not available in the kernel((\n");
    }
    
    add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);

    while (true)
    {
        struct io_uring_cqe *cqe;
        int ret;
        struct io_uring_cqe *cqes[DEFAULT_BACKLOG_SIZE];

        io_uring_submit(&ring);

        ret = io_uring_wait_cqe(&ring, &cqe);
        assert(ret == 0);

        cqe_count = io_uring_peek_batch_cqe(&ring, cqes, sizeof(cqes) / sizeof(cqes[0]));

        for (int i = 0; i < cqe_count; i++) {
            struct conn_info *user_data;
            unsigned type;
            cqe = cqes[i];
            user_data = (struct conn_info *) io_uring_cqe_get_data(cqe);
            type = user_data->type;

            if (type == ACCEPT) {
                int sock_conn_fd = cqe->res;
                add_socket_read(&ring, sock_conn_fd, MAX_MESSAGE_LEN);
                add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);
            } else if (type == READ) {
                int bytes_read = cqe->res;
                if (bytes_read <= 0) {
                    elog(LOG, "-------shutdown--------\n");
                    shutdown(user_data->fd, SHUT_RDWR);
                } else {
                    //parse input
                    int outputSize;
                    parse_input((char*)&bufs[user_data->fd], bytes_read, &outputSize, user_data->fd);
                    add_socket_write(&ring, user_data->fd, outputSize);
                }
            } else if (type == WRITE) {
                add_socket_read(&ring, user_data->fd, MAX_MESSAGE_LEN);
                // elog(LOG, "shutdown socket on fd: %d", user_data->fd);
                // shutdown(user_data->fd, SHUT_RDWR);
            }
            io_uring_cqe_seen(&ring, cqe);
        }
    }

socket_listen_fail:
socket_bind_fail:
    close(listen_socket);
    return;
}

// int 
// create_connection(PGconn** conn, char* conn_info) {
//     *conn = PQconnectdb(conn_info);

//     if (PQstatus(*conn) != CONNECTION_OK) {
//         elog(ERROR, "Error while connecting to the database server: %s\n", PQerrorMessage(*conn));
//         PQfinish(*conn);
//         return 0;
//     }
//     // We have successfully established a connection to the database server
//     elog(LOG, "Connection Established\n");
//     elog(LOG, "Port: %s\n", PQport(*conn));
//     elog(LOG, "Host: %s\n", PQhost(*conn));
//     elog(LOG, "DBName: %s\n", PQdb(*conn));
//     return 1;
// }

// void
// close_connection(PGconn** conn) {
//     PQfinish(*conn);
//     elog(LOG, "Libpq connection closed");
// }

// int
// exec_query(PGconn** conn, char *query, PGresult** res) {
//     ExecStatusType resStatus;
//     elog(LOG, "Start execution query: %s", query);
//     *res = PQexec(*conn, query);
//     resStatus = PQresultStatus(*res);
//     // convert status to string and log
//     elog(LOG, "Finish execution query with status: %s", PQresStatus(resStatus));

//     //PGRES_COMMAN_OK - Successful completion of a command returning no data
//     //PGRES_TUPLES_OK - Successful completion of a command returning data (such as a SELECT or SHOW)
//     if (resStatus != PGRES_TUPLES_OK && resStatus != PGRES_COMMAND_OK) {
//         elog(ERROR, "Error while executing the query: %s", PQerrorMessage(*conn));
//         PQclear(*res);
//         return 0;
//     }
//     return 1;
// }

// void test_connect(void) {
//     char *query = "CREATE TABLE Person(pk_Person SERIAL PRIMARY KEY, id UUID NOT NULL, name TEXT ARRAY NOT NULL);";
//     char *conn_info = "dbname=postgres host=localhost port=5432";
//     int rows, cols;
//     PGconn *conn;
//     PGresult *res;
//     if (!create_connection(&conn, conn_info)) {
//         return;
//     }

//     // char* query = "SELECT * FROM table1;";
//     //it is possible to exec many commands like "INSERT INTO table1 values(5); SELECT * FROM table1;"
//     // exec_query(&conn, "SELECT * FROM table1;", &res);
//     exec_query(&conn, query, &res);
//     rows = PQntuples(res);
//     cols = PQnfields(res);

//     elog(LOG, "Number of rows: %d\n", rows);
//     elog(LOG, "Number of columns: %d\n", cols);
//     // Print the column names
//     for (int i = 0; i < cols; i++) {
//         elog(LOG, "%s\t", PQfname(res, i));
//     }

//     // Print all the rows and columns
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//         // Print the column value
//             elog(LOG, "%s\t", PQgetvalue(res, i, j));
//         }
//         elog(LOG, "-------------------------------------------------------");
//     }

//     //clear used resources
//     PQclear(res);
//     close_connection(&conn);
// }