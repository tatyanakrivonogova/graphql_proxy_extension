#include "defines.h"
#include "config/config.h"
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


PG_MODULE_MAGIC;
static void graphql_proxy_start_worker(void);
PGDLLEXPORT void graphql_proxy_main(Datum main_arg);
void shutdown_graphql_proxy_server();
void sigterm_handler(int sig);
void sigquit_handler(int sig);

ConfigEntry *config;
size_t num_entries;

PGconn *pg_conn;

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

    if (resolvers) {
        hashmap_free_all(resolvers);
        resolvers = NULL;
        elog(LOG, "shutdown_graphql_proxy_server(): hashmap resolvers is free\n");
    }

    if (json_schema) {
        free((char *) json_schema);
        json_schema = NULL;
        elog(LOG, "shutdown_graphql_proxy_server(): json schema is free");
    }

    // close clients sockets and connections to database
    close_conns();
    PQfinish(pg_conn);
    
    // free config entries
    if (config) {
        free_config(config, num_entries);
        elog(LOG, "shutdown_graphql_proxy_server(): config is free");
    }

    elog(LOG, "graphql_proxy_main(): --------------shutdown proxy server-----------------\n");
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
    int port = 0;
    int backlog_size = 0;
    int max_entries = 0;
    char *file_schema = NULL;
    char *file_types_reflection = NULL;
    char *db_name = NULL;
    char *db_host = NULL;
    int db_port = 0;
    char *resolvers_filename = NULL;

    struct io_uring_params params;
    struct io_uring ring;
    int cqe_count;
    const int val = 1;

    struct sockaddr_in client_addr;
    socklen_t client_len;

    // set signal handlers
    pqsignal(SIGTERM, sigterm_handler);
    pqsignal(SIGQUIT, sigquit_handler);
    BackgroundWorkerUnblockSignals();

    client_len = sizeof(client_addr);

    // load config
    config = load_config_file("../contrib/graphql_proxy/proxy.conf", &num_entries);
    if (config == NULL) {
        ereport(LOG, errmsg("graphql_proxy_main(): config loading failed\n"));
        shutdown_graphql_proxy_server();
    }

    // get port value from config
    port = get_int_value(get_config_value("port", config, num_entries));
    if (port == 0) {
        port = DEFAULT_PROXY_PORT;
    }
    elog(LOG, "graphql_proxy_main(): port=%d\n", port);
    struct sockaddr_in sockaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY,
    };

    // get file with schema
    file_schema = get_config_value("schema", config, num_entries);
    if (file_schema == 0) {
        ereport(LOG, errmsg("graphql_proxy_main(): File with schema is not specified\n"));
        shutdown_graphql_proxy_server();
    }
    // get json schema
    json_schema = schema_to_json(file_schema);
    if (!json_schema) {
        ereport(LOG, errmsg("graphql_proxy_main(): Getting json representation of schema failed\n"));
        shutdown_graphql_proxy_server();
    }

    // get file with reflection graphql types to postgresql types
    file_types_reflection = get_config_value("types_reflection", config, num_entries);
    if (file_types_reflection == 0) {
        ereport(LOG, errmsg("graphql_proxy_main(): File with types reflection is not specified\n"));
        shutdown_graphql_proxy_server();
    }
    // get params for connecting to database
    db_name = get_config_value("db_name", config, num_entries);
    if (db_name == 0) {
        ereport(LOG, errmsg("graphql_proxy_main(): Database name is not specified\n"));
        db_name = DEFAULT_DB_NAME;
    }
    db_host = get_config_value("db_host", config, num_entries);
    if (db_host == 0) {
        ereport(LOG, errmsg("graphql_proxy_main(): Database host is not specified\n"));
        db_host = DEFAULT_DB_HOST;
    }
    db_port = get_int_value(get_config_value("db_port", config, num_entries));
    if (db_port == 0) {
        ereport(LOG, errmsg("graphql_proxy_main(): Database port is not specified\n"));
        db_port = DEFAULT_DB_PORT;
    }
    resolvers_filename = get_config_value("resolvers", config, num_entries);
    if (resolvers_filename == 0) {
        ereport(LOG, errmsg("graphql_proxy_main(): File with resolvers is not specified\n"));
        shutdown_graphql_proxy_server();
    }

    char conn_info[256];
    // create connection to PostgresQL database
    snprintf(conn_info, 256, "dbname=%s host=%s port=%d", db_name, db_host, db_port);
    if (!create_connection(&pg_conn, conn_info)) {
        shutdown_graphql_proxy_server();
    }

    // converting GraphQL schema to PostgresQL schema
    resolvers = schema_convert(json_schema, file_types_reflection, resolvers_filename);
    if (!resolvers) {
        ereport(LOG, errmsg("graphql_proxy_main(): Schema converting failed\n"));
        shutdown_graphql_proxy_server();
    }
    free((char *)json_schema);
    json_schema = NULL;

    // create server socket
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    elog(LOG, "graphql_proxy_main(): listen_socket: %d\n", listen_socket);
    if (listen_socket == -1) {
        char* errorbuf = strerror(errno);
        ereport(LOG, errmsg("graphql_proxy_main(): socket() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1) {
        char* errorbuf = strerror(errno);
        ereport(LOG, errmsg("graphql_proxy_main(): setsockopt() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    if (bind(listen_socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
        char* errorbuf = strerror(errno);
        ereport(LOG, errmsg("graphql_proxy_main(): bind() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    // get backlog size value from config
    backlog_size = get_int_value(get_config_value("backlog_size", config, num_entries));
    if (backlog_size == 0) {
        backlog_size = DEFAULT_BACKLOG_SIZE;
    }
    elog(LOG, "graphql_proxy_main(): backlog_size=%d\n", backlog_size);
    if (listen(listen_socket, backlog_size)) {
        char* errorbuf = strerror(errno);
        ereport(LOG, errmsg("graphql_proxy_main(): listen() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    memset(&params, 0, sizeof(params));
    // get max entries value from config
    max_entries = get_int_value(get_config_value("max_entries", config, num_entries));
    if (max_entries == 0) {
        max_entries = DEFAULT_MAX_ENTRIES;
    }
    elog(LOG, "graphql_proxy_main(): max_entries=%d\n", max_entries);

    if (io_uring_queue_init_params(max_entries, &ring, &params)) {
        char* errorbuf = strerror(errno);
        ereport(LOG, errmsg("graphql_proxy_main(): uring_init_params() error\nError: %s\n", errorbuf));
        shutdown_graphql_proxy_server();
    }

    if (!(params.features & IORING_FEAT_FAST_POLL)) {
        ereport(LOG, errmsg("graphql_proxy_main(): IORING_FEAT_FAST_POLL is not available in the kernel\n"));
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
            ereport(LOG, errmsg("graphql_proxy_main(): io_uring_wait_cqe() error\nError: %s\n", errorbuf));
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