#include "executor/spi.h"

#include <libpq/libpq.h>
#include <libpq/libpq-fs.h>
#include <postgresql/libpq-fe.h>

int create_connection(PGconn** conn, char* conn_info);
void close_connection(PGconn** conn);
int exec_query(PGconn** conn, char *query, PGresult** res);
void handle_query(PGresult* res);