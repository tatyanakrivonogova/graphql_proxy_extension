#include "event_handling.h"

#include "postgres.h"
#include "multiple_user_access.h"

#include <liburing.h>
#include <unistd.h>

char bufs[MAX_CONNECTIONS][MAX_MESSAGE_LEN];
struct conn_info conns[MAX_CONNECTIONS];

void
add_accept(struct io_uring *ring, int fd, struct sockaddr *client_addr, socklen_t *client_len) {
    int index;
    int res;
    conn_info *conn_i;
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);
    io_uring_prep_accept(sqe, fd, client_addr, client_len, 0);

    if (!reserve_conn_structure(fd)){
        elog(ERROR, "add_accept(): can not reserve conn structure");
        abort(); // remove!!!
    }

    index = 0;
    res = get_conn_index(fd, &index);
    elog(LOG, "add_accept(): get_conn_index finish with status: %d, index: %d", res, index);
    if (!res) {
        elog(LOG, "add_accept(): index is not reserved, try to reserve");
        reserve_conn_structure(fd); // add check return value
        get_conn_index(fd, &index);
    }
    print_conns();
    conn_i = &conns[index];
    conn_i->type = ACCEPT;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
add_socket_read(struct io_uring *ring, int fd, size_t size) {
    struct io_uring_sqe *sqe;
    conn_info *conn_i;
    int index;
    int res;

    elog(LOG, "add_socket_read(): Start socket_read");
    memset(&bufs[fd], 0, size);
    sqe = io_uring_get_sqe(ring);
    io_uring_prep_recv(sqe, fd, &bufs[fd], size, 0);
    elog(LOG, "add_socket_read(): Read buf from fd = %d: %s, size: %ld", fd, (char*)&bufs[fd], size);

    index = 0;
    res = get_conn_index(fd, &index);
    elog(LOG, "add_socket_read(): get_conn_index finish with status: %d, index: %d", res, index);
    if (!res) {
        elog(LOG, "add_socket_read(): can not find reserved index, try to reserve");
        reserve_conn_structure(fd); // add check return value
        get_conn_index(fd, &index);
    }
    elog(LOG, "add_socket_read(): get_conn_index finish with status: %d, index: %d", res, index);
    conn_i = &conns[index];
    conn_i->type = READ;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
add_socket_write(struct io_uring *ring, int fd, size_t size) {
    conn_info *conn_i;
    struct io_uring_sqe *sqe;
    int index;
    int res; 

    elog(LOG, "add_socket_write(): Start socket_write");
    elog(LOG, "add_socket_write(): Write buf into fd = %d: %s, size: %ld", fd, (char*)&bufs[fd], size);
    sqe = io_uring_get_sqe(ring);
    elog(LOG, "add_socket_write(): Get uring sqe done");
    io_uring_prep_send(sqe, fd, &bufs[fd], size, 0);

    index = 0;
    res = get_conn_index(fd, &index);
    elog(LOG, "add_socket_write(): get_conn_index finish with status: %d, index: %d", res, index);
    if (!res) {
        elog(LOG, "add_socket_write(): can not find right index, try to reserve");
        reserve_conn_structure(fd); // add check return value
        get_conn_index(fd, &index);
    }
    conn_i = &conns[index];
    conn_i->type = WRITE;

    io_uring_sqe_set_data(sqe, conn_i);
}

void
socket_close(conn_info *user_data, int how) {
    shutdown(user_data->fd, how);
    close(user_data->fd);
    free_conn_index(user_data->fd); 
}
