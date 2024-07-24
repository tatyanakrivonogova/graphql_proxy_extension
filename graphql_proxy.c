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

#define DEFAULT_PORT            (7879)
#define DEFAULT_BACKLOG_SIZE    (512)
#define MAX_ENTRIES          (10)

PG_MODULE_MAGIC;
static void graphql_proxy_start_worker(void);
PGDLLEXPORT void graphql_proxy_main(Datum main_arg);

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

int print_entry(const void* key, size_t ksize, uintptr_t value, void* usr)
{
	elog(LOG, "Entry \"%s\": %s\n", (char *)key, (char *)value);
    return 0;
}

void
graphql_proxy_main(Datum main_arg) {
    struct io_uring_params params;
    struct io_uring ring;
    int cqe_count;
    int listen_socket;
    const int val = 1;
    int error;

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
    hashmap *resolvers = schema_convert(json_schema);

    error = hashmap_iterate(resolvers, print_entry, NULL);
    if (error == -1)
        elog(LOG, "!!!---------hashmap_iterate error\n");

    // hashmap_free(resolvers);


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
        int ret;
        struct io_uring_cqe *cqe;
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
                add_accept(&ring, listen_socket, (struct sockaddr *) &client_addr, &client_len);
                add_socket_read(&ring, sock_conn_fd, MAX_MESSAGE_LEN);
            } else if (type == READ) {
                int bytes_read = cqe->res;
                if (bytes_read <= 0) {
                    elog(LOG, "-------socket shutdown--------\n");
                    socket_close(user_data, SHUT_RDWR);
                    // shutdown(user_data->fd, SHUT_RDWR);
                    // free_conn_index(user_data->fd); 
                } else {
                    //parse input
                    int outputSize;
                    parse_input((char*)&bufs[user_data->fd], bytes_read, &outputSize, user_data->fd, resolvers);
                    add_socket_write(&ring, user_data->fd, outputSize);
                    printConns();
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