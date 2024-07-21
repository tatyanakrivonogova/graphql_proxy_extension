#include "event_handling.h"

#include "postgres.h"

#include <liburing.h>


void
add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len) {
    conn_info *conn_i;
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_accept(sqe, fd, client_addr, client_len, 0);

    conn_i = &conns[fd];
    conn_i->fd = fd;
    conn_i->type = ACCEPT;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
add_socket_read(struct io_uring *ring, int fd, size_t size) {
    struct io_uring_sqe *sqe;
    conn_info *conn_i;

    elog(LOG, "Start socket_read");
    sqe = io_uring_get_sqe(ring);
    io_uring_prep_recv(sqe, fd, &bufs[fd], size, 0);
    elog(LOG, "Read buf from fd = %d: %s, size: %ld", fd, (char*)&bufs[fd], size);

    conn_i = &conns[fd];
    conn_i->fd = fd;
    conn_i->type = READ;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
add_socket_write(struct io_uring *ring, int fd, size_t size) {
    conn_info *conn_i;
    struct io_uring_sqe *sqe;

    elog(LOG, "Start socket_write");
    elog(LOG, "Write buf into fd = %d: %s, size: %ld", fd, (char*)&bufs[fd], size);
    sqe = io_uring_get_sqe(ring);
    elog(LOG, "Get uring sqe done");
    io_uring_prep_send(sqe, fd, &bufs[fd], size, 0);

    conn_i = &conns[fd];
    conn_i->fd = fd;
    conn_i->type = WRITE;

    io_uring_sqe_set_data(sqe, conn_i);
}