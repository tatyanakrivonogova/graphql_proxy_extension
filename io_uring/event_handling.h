#include <liburing.h>
#include "postgresql/libpq-fe.h"

#define MAX_CONNECTIONS      (20)
#define MAX_MESSAGE_LEN      (10000)


typedef struct conn_info {
    PGconn *pg_conn;
    PGresult *pg_res;
    int fd;
    unsigned type;
} conn_info;

extern char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];
extern struct conn_info conns[MAX_CONNECTIONS];

enum {
    ACCEPT,
    READ,
    WRITE,
};

void
add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len);

void
add_socket_read(struct io_uring *ring, int fd, size_t size);

void
add_socket_write(struct io_uring *ring, int fd, size_t size);

void
socket_close(conn_info *user_data, int how);