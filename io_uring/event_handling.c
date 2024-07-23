#include "event_handling.h"

#include "postgres.h"
#include "multiple_user_access.h"

#include <liburing.h>

char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];
struct conn_info conns[MAX_CONNECTIONS];

void
add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len) {
    
    conn_info *conn_i;
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_accept(sqe, fd, client_addr, client_len, 0);

    if(!reserve_conn_structure(fd)){
        elog(ERROR, "can not reserve conn structure");
        abort();
    }

    int index = 0;
    int res; 
    res = get_conn_index(fd, &index);
    elog(LOG, "get_conn_index finish with status: %d, index: %d", res, index);
    if (!res) {
        elog(LOG, "index is not reserved, try to reserve");
        reserve_conn_structure(fd);
        get_conn_index(fd, &index);
    }
    printConns();
    conn_i = &conns[index];
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

    int index = 0;
    int res;
    res = get_conn_index(fd, &index);
    elog(LOG, "get_conn_index finish with status: %d, index: %d", res, index);
    if (!res) {
        elog(LOG, "can not find reserved index, try to reserve");
        reserve_conn_structure(fd);
        get_conn_index(fd, &index);
    }
    elog(LOG, "get_conn_index finish with status: %d, index: %d", res, index);
    conn_i = &conns[index];
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

    int index = 0;
    int res; 
    res = get_conn_index(fd, &index);
    elog(LOG, "get_conn_index finish with status: %d, index: %d", res, index);
    if (!res) {
        elog(LOG, "can not find right index, try to reserve");
        reserve_conn_structure(fd);
        get_conn_index(fd, &index);
    }
    conn_i = &conns[index];
    // conn_i->fd = fd;
    conn_i->type = WRITE;

    io_uring_sqe_set_data(sqe, conn_i);
}