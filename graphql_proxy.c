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

#define DEFAULT_PORT            (7879)
#define DEFAULT_BACKLOG_SIZE    (512)
#define BUFFER_SIZE             (1024)
#define NUM_HEADERS             (100)

#define error_event(events)     \
    ((events) & EV_ERROR)
#define read_available(events)  \
    ((events) & EV_READ)

static struct ev_loop *loop;
static struct coro_context main_context;
static struct coro_context server_context;
static struct coro_context client_context;

PG_MODULE_MAGIC;
PGDLLEXPORT void graphql_proxy_main(Datum main_arg);

static void
graphql_proxy_start_worker(void)
{
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
_PG_init(void)
{
	graphql_proxy_start_worker();
}

static bool socket_set_nonblock(int socket_fd) {
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags < 0) {
        return false;
    }

    if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return false;
    }

    return true;
}

static void on_client_wakeup_cb(EV_P_ struct ev_io *io_handle, int revents) {
    elog(LOG, "coroutine switch: server ---> client\n");
    coro_transfer(&server_context, &client_context);
}

static void client_coroutine_main(void *arg) {
    struct ev_io *io_handle = (struct ev_io *)arg;

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

    char* request = (char*)malloc(BUFFER_SIZE);
    size_t request_len;
    
    for(;;) {
        res = read(io_handle->fd, request, BUFFER_SIZE);

        if (!res) {
            elog(LOG, "QraphQL Proxy: client disconnected\n");
            ev_io_stop(loop, io_handle);
            close(io_handle->fd);
            free(io_handle);
            coro_transfer(&client_context, &server_context);
            return;
        }

        if (res < 0) {
            if (errno == EAGAIN) {
                elog(LOG, "coroutine switch: client ---> server\n");
                coro_transfer(&client_context, &server_context);
                continue;
            }

            ev_io_stop(loop, io_handle);
            close(io_handle->fd);
            free(io_handle);
            return;
        }
        
        //parsing of request
        request_len = res;
        elog(LOG, "read from client: %ld\n", request_len);
        num_headers = NUM_HEADERS;
        err = phr_parse_request(request, request_len, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
        if (err == -1 || err == -2) {
            printf("send_request_to_server(): failed while parse HTTP request, error %d\n", err);
            res = write(io_handle->fd, "Failed while parse HTTP request. Change and try again\n", sizeof("Failed while parse HTTP resuest. Change and try again\n"));
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
        }
        res = write(io_handle->fd, query, (size_t)query_len);
        elog(LOG, "query sent to client\n");

        // close connection after completing request
close_client_connection:
        ev_io_stop(loop, io_handle);
        close(io_handle->fd);

        if (parsed_method) free(parsed_method);
        if (parsed_path) free(parsed_path);
        if (parsed_header_name) free(parsed_header_name);
        if (parsed_header_value) free(parsed_header_value);
        if (query) free(query);
        elog(LOG, "memory is free\n");
        coro_transfer(&client_context, &server_context);
        return;
    }
}

static bool client_coroutine_create(struct ev_io *io_handle) {
    struct coro_stack client_coroutine_stack;
    
    if (!coro_stack_alloc(&client_coroutine_stack, 4096)) {
        return false;
    }

    coro_create(&client_context, client_coroutine_main, (void *)io_handle,
                    client_coroutine_stack.sptr, client_coroutine_stack.ssze);
    
    return true;
}

static void on_accept_cb(EV_P_ struct ev_io *io_handle, int revents) {
    struct ev_io *client_io_handle;
    int flag;
    int socket_fd = accept(io_handle->fd, NULL, NULL);

    if (socket_fd == -1) {
        elog(ERROR, "on_accept_cb() error\n");
        abort();
    }

    elog(LOG, "set_nonblock(): %d\n", socket_fd);
    if (!socket_set_nonblock(socket_fd)) {
        close(socket_fd);
        return;
    }

    flag = 1;
    if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) < 0) {
        perror("setsockopt() error\n");
        close(socket_fd);
        return;
    }

    elog(LOG, "GraphQL Proxy: client connected\n");

    client_io_handle = (struct ev_io *)
                                        malloc(sizeof(*client_io_handle));
    if (!client_io_handle) {
        close(socket_fd);
        return;
    }

    if (!client_coroutine_create(client_io_handle)) {
        close(socket_fd);
        free(client_io_handle);
        return;
    }

    ev_io_init(client_io_handle, on_client_wakeup_cb, socket_fd, EV_READ);
    ev_io_start(loop, client_io_handle);

    coro_transfer(&server_context, &client_context);
}

static void server_coroutine_main(void *arg) {
    int listen_socket;
    struct ev_io *accept_io_handle;
    struct sockaddr_in sockaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(DEFAULT_PORT),
        .sin_addr.s_addr = INADDR_ANY,
    };

    loop = ev_default_loop(0);
    
    if (!loop) {
        elog(ERROR, "cannot create libev default loop\n");
        return;
    }

    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == -1) {
        elog(ERROR, "socket() error\n");
        ev_loop_destroy(loop);
        return;
    }

    elog(LOG, "set_nonblock(): %d\n", listen_socket);
    if (!socket_set_nonblock(listen_socket)) {
        elog(ERROR, "for listen socket set nonblocking mode fail\n");
        goto socket_set_nonblock_fail;
    }

    if (bind(listen_socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) {
        elog(ERROR, "bind() error\n");
        goto socket_bind_fail;
    }

    if (listen(listen_socket, DEFAULT_BACKLOG_SIZE)) {
        elog(ERROR, "listen() error\n");
        goto socket_listen_fail;
    }

    accept_io_handle = (struct ev_io *)
                                            malloc(sizeof(*accept_io_handle));
    if (!accept_io_handle) {
        elog(ERROR, "cannot create io handle for listen socket\n");
        goto accept_io_handle_alloc_fail;
    }

    ev_io_init(accept_io_handle, on_accept_cb, listen_socket, EV_READ);
    ev_io_start(loop, accept_io_handle);

    ev_run(loop, 0);

accept_io_handle_alloc_fail:
socket_listen_fail:
socket_bind_fail:
socket_set_nonblock_fail:
    close(listen_socket);
    ev_loop_destroy(loop);
}

static bool server_coroutine_create(void) {
    struct coro_stack server_coroutine_stack;
    coro_create(&main_context, NULL, NULL, NULL, -1);

    if (!coro_stack_alloc(&server_coroutine_stack, 4096)) {
        return false;
    }

    coro_create(&server_context, server_coroutine_main, NULL,
                    server_coroutine_stack.sptr, server_coroutine_stack.ssze);
    
    return true;
}

void
graphql_proxy_main(Datum main_arg)
{
    if (!server_coroutine_create()) {
        elog(ERROR, "cannot init server coro context\n");
        return;
    }

    coro_transfer(&main_context, &server_context);
    return;
}