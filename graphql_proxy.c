#include "io_uring/event_handling.h"
#include "http/input_parsing.h"
#include "schema/schema_converting.h"
#include "libgraphqlparser/schema_to_json.h"
#include "hashmap/map.h"
#include "io_uring/multiple_user_access.h"
#include "postgres_connect/postgres_connect.h"

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "executor/executor.h"
#include "postmaster/bgworker.h"
#include "miscadmin.h"
#include "storage/proc.h"
#include "postmaster/interrupt.h"
#include "tcop/tcopprot.h"
#include "storage/ipc.h"

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


#define DEFAULT_PORT            (7879)
#define DEFAULT_BACKLOG_SIZE    (512)
#define MAX_ENTRIES          (10)

PG_MODULE_MAGIC;
static void graphql_proxy_start_worker(void);
PGDLLEXPORT void graphql_proxy_main(Datum main_arg);
void shutdown_graphql_proxy_server();
void sigterm_handler(int sig);
void sigquit_handler(int sig);

hashmap *resolvers;
int listen_socket = 0;
const char *json_schema = NULL;


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

void shutdown_graphql_proxy_server() {
    if ((listen_socket != 0) && (listen_socket != -1)) close(listen_socket);
    listen_socket = 0;

    if (resolvers) hashmap_free(resolvers);
    resolvers = NULL;

    if (json_schema) free((char *) json_schema);
    json_schema = NULL;

    close_conns();

    proc_exit(0);
}

void sigterm_handler(int sig) {
    if (sig == SIGTERM) {
        elog(LOG, "graphql_proxy_main(): ----------Received SIGTERM signal---------------");
        shutdown_graphql_proxy_server();   
    } 
}

void sigquit_handler(int sig) {
    if (sig == SIGQUIT) {
        elog(LOG, "graphql_proxy_main(): ----------Received SIGQUIT signal----------------");
        shutdown_graphql_proxy_server();
    }
}

void
graphql_proxy_main(Datum main_arg) {
    struct io_uring_params params;
    struct io_uring ring;
    int cqe_count;
    const int val = 1;

    struct sockaddr_in sockaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(DEFAULT_PORT),
        .sin_addr.s_addr = INADDR_ANY,
    };
    struct sockaddr_in client_addr;
    socklen_t client_len;

    // set signal handlers
    pqsignal(SIGTERM, sigterm_handler);
    pqsignal(SIGQUIT, sigquit_handler);
    BackgroundWorkerUnblockSignals();

    client_len = sizeof(client_addr);

    // get json schema
    json_schema = schema_to_json("../contrib/graphql_proxy/libgraphqlparser/schema.graphql");
    if (!json_schema) {
        ereport(ERROR, errmsg("graphql_proxy_main(): Getting json representation of schema failed\n"));
        shutdown_graphql_proxy_server();
    }

    // converting GraphQL schema to PostgresQL schema
    resolvers = schema_convert(json_schema);
    free((char *)json_schema);
    json_schema = NULL;

    // create server socket
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == -1) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("graphql_proxy_main(): socket() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("graphql_proxy_main(): setsockopt() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    if (bind(listen_socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("graphql_proxy_main(): bind() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    // to do: DEFAULT_BACKLOG_SIZE from config
    if (listen(listen_socket, DEFAULT_BACKLOG_SIZE)) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("graphql_proxy_main(): listen() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    memset(&params, 0, sizeof(params));
    if (io_uring_queue_init_params(MAX_ENTRIES, &ring, &params)) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("graphql_proxy_main(): uring_init_params() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    if (!(params.features & IORING_FEAT_FAST_POLL)) {
        ereport(ERROR, errmsg("graphql_proxy_main(): IORING_FEAT_FAST_POLL is not available in the kernel\n"));
        shutdown_graphql_proxy_server();
    }
    
    // start accepting clients
    add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);

    while (true)
    {
        struct io_uring_cqe *cqe;
        struct io_uring_cqe *cqes[DEFAULT_BACKLOG_SIZE];

        // send sqes to execute in the kernel
        io_uring_submit(&ring);

        // wait for cqes
        if (io_uring_wait_cqe(&ring, &cqe) != 0) {
            char* errorbuf = strerror(errno);
            ereport(ERROR, errmsg("graphql_proxy_main(): io_uring_wait_cqe() error\nError: %s\n", errorbuf));
            shutdown_graphql_proxy_server();
        }

        // get array of cqes
        cqe_count = io_uring_peek_batch_cqe(&ring, cqes, sizeof(cqes) / sizeof(cqes[0]));

        for (int i = 0; i < cqe_count; i++) {
            struct conn_info *user_data;
            unsigned type;
            cqe = cqes[i];
            user_data = (struct conn_info *) io_uring_cqe_get_data(cqe);
            type = user_data->type;

            if (type == ACCEPT) {
                int sock_conn_fd = cqe->res;
                add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);
                add_socket_read(&ring, sock_conn_fd, MAX_MESSAGE_LEN);
            } else if (type == READ) {
                int bytes_read = cqe->res;
                if (bytes_read <= 0) {
                    elog(LOG, "graphql_proxy_main(): client %d disconnected\n", user_data->fd);
                    socket_close(user_data, SHUT_RDWR);
                } else {
                    // handle http-request
                    int outputSize;
                    parse_input((char*)&bufs[user_data->fd], bytes_read, &outputSize, user_data->fd, resolvers);
                    add_socket_write(&ring, user_data->fd, outputSize);
                }
            } else if (type == WRITE) {
                add_socket_read(&ring, user_data->fd, MAX_MESSAGE_LEN);
            }

            // mark cqe as handled
            io_uring_cqe_seen(&ring, cqe);
        }
    }
    return;
}