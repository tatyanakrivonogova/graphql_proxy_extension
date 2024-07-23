#include <postgresql/libpq-fe.h>

void 
test_connect(void);

int 
create_connection(PGconn** conn, char* conn_info);

void
close_connection(PGconn** conn);

int
exec_query(PGconn** conn, char *query, PGresult** res);
