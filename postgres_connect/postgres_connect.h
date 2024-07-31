#include <postgresql/libpq-fe.h>

#define RESPONSE_LENGTH 4096

void
clearRes(PGresult** res);

int 
create_connection(PGconn** pg_conn, char* conn_info);

void
close_connection(PGresult **pg_res);

PGresult* prepare_statement(PGconn* pg_conn, const char *stmt_name, const char *sql);

PGresult* execute_prepared_statement(PGconn *pg_conn, 
        const char *stmt_name, const char **paramValues, int paramCount);

int
exec_query(PGconn** pg_conn, char *query, PGresult** res);

char * 
handle_query(PGresult** res, int *server_error);
