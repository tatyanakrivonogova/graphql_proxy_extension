#include <postgresql/libpq-fe.h>

#define RESPONSE_LENGTH 4096

void 
test_connect(void);

void
clearRes(PGresult** res);

int 
create_connection(PGconn** pg_conn, char* conn_info);

void
close_connection(PGconn** pg_conn, PGresult **pg_res);

int
exec_query(PGconn** pg_conn, char *query, PGresult** res);

void 
handle_query(PGresult** res);
