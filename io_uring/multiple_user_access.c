#include "multiple_user_access.h"

#include "postgres.h"
#include "event_handling.h"
#include "postgres_connect/postgres_connect.h"

int
reserve_conn_structure(int fd) {
    int res;
    int index;
    elog(LOG, "reserve conn for fd: %d", fd);
    res = get_conn_index(fd, &index);
    if (res) {
        elog(LOG, "index for fd: %d is reserved - %d", fd, res);
        goto reserve_done;
    }

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        elog(LOG, "try to reserve conns[%d].fd = %d", i, conns[i].fd);
        if (conns[i].fd == 0) {
            conns[i].fd = fd;
            index = i;
            elog(LOG, "reserved index: %d", i);
            goto reserve_done;
        }
    }
    elog(LOG, "no free conns");
reserve_error:
    elog(LOG, "can not reserve conn structure");
    return 0;
reserve_done:
    elog(LOG, "Connecting to database...\n");
    char *conn_info = "dbname=postgres host=localhost port=5432";
    if (!create_connection(&conns[index].pg_conn, conn_info)) {
        free_conn_index(fd);
        goto reserve_error;
    }
    return 1;
}

int
get_conn_index(int fd, int *index) {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd == fd) {
            elog(LOG, "get done, index: %d", i);
            *index = i;
            return 1;
        }
    }
    elog(LOG, "get did not find index");
    return 0;
}

void
free_conn_index(int fd) {
    elog(LOG, "free conn_index for fd: %d", fd);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd == fd) {
            conns[i].fd = 0;
            close_connection(&conns[i].pg_conn, &conns[i].pg_res);
            return;
        }
    }
}

void
printConns() {
    for (int i = 0; i < MAX_CONNECTIONS; i++)
        elog(LOG, "conn fd: %d", conns[i].fd);
}

void closeConns() {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd != 0) socket_close(conns + i, SHUT_RDWR);
    }   
}