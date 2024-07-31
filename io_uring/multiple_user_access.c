#include "multiple_user_access.h"

#include "postgres.h"
#include "event_handling.h"
#include "postgres_connect/postgres_connect.h"

int
reserve_conn_structure(int fd) {
    int res;
    int index;
    
    elog(LOG, "reserve_conn_structure(): reserve conn for fd: %d", fd);

    res = get_conn_index(fd, &index);
    if (res) {
        elog(LOG, "reserve_conn_structure(): index for fd: %d is reserved - %d", fd, res);
        return 1;
    }

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        elog(LOG, "reserve_conn_structure(): try to reserve conns[%d].fd = %d", i, conns[i].fd);
        if (conns[i].fd == 0) {
            conns[i].fd = fd;
            index = i;
            elog(LOG, "reserve_conn_structure(): reserved index: %d", i);
            return 1;
        }
    }
    elog(LOG, "reserve_conn_structure(): no free conns");
    elog(LOG, "reserve_conn_structure(): can not reserve conn structure");
    return 0;
}

int
get_conn_index(int fd, int *index) {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd == fd) {
            elog(LOG, "get_conn_index(): index is found: %d", i);
            *index = i;
            return 1;
        }
    }
    elog(LOG, "get_conn_index(): connection index is not found");
    return 0;
}

void
free_conn_index(int fd) {
    elog(LOG, "free_conn_index(): free conn_index for fd: %d", fd);
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd == fd) {
            conns[i].fd = 0;
            close_connection(&conns[i].pg_res);
            return;
        }
    }
}

void
print_conns() {
    for (int i = 0; i < MAX_CONNECTIONS; i++)
        elog(LOG, "print_conns(): conn fd %d", conns[i].fd);
}

void 
close_conns() {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (conns[i].fd != 0) {
            socket_close(conns + i, SHUT_RDWR);
        }
    }   
}