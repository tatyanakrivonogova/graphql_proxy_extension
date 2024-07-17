#include "postgres.h"

#include "coro.h"
#include "http_parser.h"
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
#include <ev.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <liburing.h>
#include <netinet/in.h>

#define DEFAULT_PORT            (7879)
#define DEFAULT_BACKLOG_SIZE    (512)
#define NUM_HEADERS             (100)
#define MAX_CONNECTIONS      (10)
#define MAX_ENTRIES          (10)
#define MAX_MESSAGE_LEN      (512)

#define error_event(events)     \
    ((events) & EV_ERROR)
#define read_available(events)  \
    ((events) & EV_READ)

typedef struct conn_info {
    int fd;
    unsigned type;
} conn_info;

static bool serv_logs = true;
static ExecutorStart_hook_type prev_ExecutorStart = NULL;
static struct ev_loop *loop;
static struct coro_context main_context;
static struct coro_context server_context;
static struct coro_context client_context;
conn_info conns[MAX_CONNECTIONS];
char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];

static inline void close_connection(EV_P_ struct ev_io *io_handle) {
    ev_io_stop(loop, io_handle);
    close(io_handle->fd);
    free(io_handle);
} 

enum {
    ACCEPT,
    READ,
    WRITE,
};

PG_MODULE_MAGIC;
static void server_ExecutorStart(QueryDesc *queryDesc, int eflags);
static void graphql_proxy_start_worker(void);
PGDLLEXPORT void graphql_proxy_main(Datum main_arg);
void add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len);
void add_socket_read(struct io_uring *ring, int fd, size_t size);
void add_socket_write(struct io_uring *ring, int fd, size_t size);
void parse_input(char* request, size_t request_len, int* outputSize, int fd);

void
_PG_init(void) {
    DefineCustomBoolVariable("grapghql_proxy.enabled",
							 "Enable logs on hook called.",
							 NULL,
							 &serv_logs,
							 true,
							 PGC_POSTMASTER,
							 0,
							 NULL,
							 NULL,
							 NULL);

    prev_ExecutorStart = ExecutorStart_hook;
    ExecutorStart_hook = server_ExecutorStart;
    
	graphql_proxy_start_worker();
}

static void
server_ExecutorStart(QueryDesc *queryDesc, int eflags) {
	if (prev_ExecutorStart)
		prev_ExecutorStart(queryDesc, eflags);
	else
		standard_ExecutorStart(queryDesc, eflags);

    if (serv_logs)
		ereport(LOG, (errmsg("serv_hool: start executing query"),
					  errdetail("query: %s", queryDesc->sourceText)));
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
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == -1) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("socket(): %s\n", errorbuf));
        return;
    }

    struct sockaddr_in sockaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(DEFAULT_PORT),
        .sin_addr.s_addr = INADDR_ANY,
    };

    const int val = 1;
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

    struct io_uring_params params;
    struct io_uring ring;
    memset(&params, 0, sizeof(params));

    if (io_uring_queue_init_params(MAX_ENTRIES, &ring, &params)) {
        char* errorbuf = strerror(errno);
        ereport(ERROR, errmsg("uring_init_params() error: %s\n", errorbuf));
    }

    // if (!(params.features & IORING_FEAT_FAST_POLL)) {
    //     elog(LOG, "IORING_FEAT_FAST_POLL is not available in the kernel((\n");
    // }
    
    add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);

    while (true)
    {
        struct io_uring_cqe *cqe;
        int ret;

        io_uring_submit(&ring);

        ret = io_uring_wait_cqe(&ring, &cqe);
        assert(ret == 0);

        struct io_uring_cqe *cqes[DEFAULT_BACKLOG_SIZE];
        int cqe_count = io_uring_peek_batch_cqe(&ring, cqes, sizeof(cqes) / sizeof(cqes[0]));

        for (int i = 0; i < cqe_count; i++) {
            cqe = cqes[i];
            struct conn_info *user_data = (struct conn_info *) io_uring_cqe_get_data(cqe);
            unsigned type = user_data->type;

            if (type == ACCEPT) {
                int sock_conn_fd = cqe->res;
                add_socket_read(&ring, sock_conn_fd, MAX_MESSAGE_LEN);
                add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);
            } else if (type == READ) {
                int bytes_read = cqe->res;
                if (bytes_read <= 0) {
                    shutdown(user_data->fd, SHUT_RDWR);
                } else {
                    //parse input
                    int outputSize;
                    parse_input(&bufs[user_data->fd], bytes_read, &outputSize, user_data->fd);
                    add_socket_write(&ring, user_data->fd, outputSize);
                }
            } else if (type == WRITE) {
                add_socket_read(&ring, user_data->fd, MAX_MESSAGE_LEN);
            }
            io_uring_cqe_seen(&ring, cqe);
        }

    }

accept_io_handle_alloc_fail:
socket_listen_fail:
socket_bind_fail:
socket_set_nonblock_fail:
    close(listen_socket);
    return;
}

void
parse_input(char* request, size_t request_len, int* outputSize, int fd) {
    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    size_t num_headers;
    struct phr_header headers[NUM_HEADERS];

    char* parsed_method;
    char* parsed_path;
    char* parsed_header_name;
    char* parsed_header_value;

    char* query_begin;
    char *query;
    size_t query_len;
    ssize_t res;
    int err;
    *outputSize = 0;
    elog(LOG, "read from client: %ld\n", request_len);
    num_headers = NUM_HEADERS;
    err = phr_parse_request(request, request_len, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    if (err == -1 || err == -2) {
        printf("send_request_to_server(): failed while parse HTTP request, error %d\n", err);
        *outputSize = sizeof("Failed while parse HTTP resuest. Change and try again\n");
        strcpy(&bufs[fd], "Failed while parse HTTP request. Change and try again\n");
        // res = write(io_handle->fd, "Failed while parse HTTP request. Change and try again\n", 
        //             sizeof("Failed while parse HTTP resuest. Change and try again\n"));
        goto close_client_connection;
    }

    parsed_method = (char*)malloc(method_len + 1);
    parsed_path = (char*)malloc(path_len + 1);
    parsed_method[method_len] = '\0';
    parsed_path[path_len] = '\0';

    strncpy(parsed_method, method, method_len);
    strncpy(parsed_path, path, path_len);

    elog(LOG, "parsed method: %ld %s\n", method_len, parsed_method);
    elog(LOG, "parsed path: %ld %s\n", path_len, parsed_path);
    elog(LOG, "parsed minor version: %d\n", minor_version);
    elog(LOG, "parsed num headers: %ld\n", num_headers);
        
    // parsing of headers
    for (size_t i = 0; i < num_headers; ++i) {
        parsed_header_name = (char*)malloc(headers[i].name_len + 1);
        parsed_header_value = (char*)malloc(headers[i].value_len + 1);
        parsed_header_name[headers[i].name_len] = '\0';
        parsed_header_value[headers[i].value_len] = '\0';
        strncpy(parsed_header_name, headers[i].name, headers[i].name_len);
        strncpy(parsed_header_value, headers[i].value, headers[i].value_len);
        elog(LOG, "parsed header: %s: %s\n", parsed_header_name, parsed_header_value);
    }

    // parsing query
    if ((query_begin = strstr(request, "\n\n")) == 0) {
        elog(LOG, "query is empty\n");
        query_len = 0;
    } else {
        query_len = request + request_len - query_begin - 2;
        query = (char*)malloc(query_len+1);
        query[query_len] = '\0';
        strncpy(query, query_begin + 2, query_len);
        elog(LOG, "query_len: %ld query: %s\n", query_len, query);
        strcpy(bufs[fd], query);
        *outputSize = query_len;
    }
    //res = write(io_handle->fd, query, (size_t)query_len);
    elog(LOG, "buffer after query pars: %s", &bufs[fd]);

    // close connection after completing request
close_client_connection:
    if (parsed_method) free(parsed_method);
    if (parsed_path) free(parsed_path);
    if (parsed_header_name) free(parsed_header_name);
    if (parsed_header_value) free(parsed_header_value);
    if (query) free(query);
    elog(LOG, "memory is free\n");
    return;
}

void
add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_accept(sqe, fd, client_addr, client_len, 0);

    conn_info *conn_i = &conns[fd];
    conn_i->fd = fd;
    conn_i->type = ACCEPT;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
add_socket_read(struct io_uring *ring, int fd, size_t size) {
    elog(LOG, "Start socket_write");
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_recv(sqe, fd, &bufs[fd], size, 0);
    elog(LOG, "Read buf from fd = %d: %s, size: %d", fd, &bufs[fd], size);

    conn_info *conn_i = &conns[fd];
    conn_i->fd = fd;
    conn_i->type = READ;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
add_socket_write(struct io_uring *ring, int fd, size_t size) {
    elog(LOG, "Start socket_write");
    elog(LOG, "Write buf into fd = %d: %s, size: %d", fd, &bufs[fd], size);
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    elog(LOG, "Get uring sqe done");
    io_uring_prep_send(sqe, fd, &bufs[fd], size, 0);

    conn_info *conn_i = &conns[fd];
    conn_i->fd = fd;
    conn_i->type = WRITE;

    io_uring_sqe_set_data(sqe, conn_i);
}