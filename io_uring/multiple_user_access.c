#include "multiple_user_access.h"

#include "postgres.h"
#include "event_handling.h"

int
reserve_conn_structure(int fd) {
    int res;
    elog(LOG, "reserve conn for fd: %d", fd);
    int index;
    res = get_conn_index(fd, &index);
    if (res) {
        elog(LOG, "index for fd: %d is reserved - %d", fd, res);
        goto reserve_done;
    }

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        elog(LOG, "try to reserve conns[%d].fd = %d", i, conns[i].fd);
        if (conns[i].fd == 0) {
            conns[i].fd = fd;
            elog(LOG, "reserved index: %d", i);
            goto reserve_done;
        }
    }
reserve_error:
    return 0;
reserve_done:
    return 1;
}

int
get_conn_index(int fd, int *index) {
    elog(LOG, "index ptr in func: %p", index);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd == fd) {
            elog(LOG, "get done, index: %d", i);
            *index = i;
            return 1;
        }
    }
    elog(LOG, "get is not done");
    return 0;
}

void
free_conn_index(int fd) {
    elog(LOG, "free conn_index for fd: %d", fd);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd == fd) {
            conns[i].fd = 0;
            return;
        }
    }
    printConns();
}

void
printConns() {
    for (int i = 0; i < MAX_CONNECTIONS; i++)
        elog(LOG, "conn fd: %d", conns[i].fd);
}