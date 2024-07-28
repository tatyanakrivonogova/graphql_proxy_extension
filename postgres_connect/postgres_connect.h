#include <postgresql/libpq-fe.h>

void 
test_connect(void);

int 
create_connection(PGconn** pg_conn, char* conn_info);

void
close_connection(PGconn** pg_conn, PGresult **pg_res);

int
exec_query(PGconn** pg_conn, char *query, PGresult** res);

void 
handle_query(PGresult* res);
