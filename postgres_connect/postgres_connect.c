#include "postgres_connect.h"

#include "postgres.h"
#include <unistd.h>

void
clearRes(PGresult** res) {
    if (*res != NULL) PQclear(*res);
    *res = NULL;
}

int 
create_connection(PGconn** pg_conn, char* conn_info) {
    *pg_conn = PQconnectdb(conn_info);

    if (PQstatus(*pg_conn) != CONNECTION_OK) {
        elog(ERROR, "create_connection(): Error while connecting to the database server: %s\n", PQerrorMessage(*pg_conn));
        PQfinish(*pg_conn);
        return 0;
    }
    // We have successfully established a connection to the database server
    elog(LOG, "\ncreate_connection(): Connection Established");
    elog(LOG, "create_connection(): Port: %s", PQport(*pg_conn));
    elog(LOG, "create_connection(): Host: %s", PQhost(*pg_conn));
    elog(LOG, "create_connection(): DBName: %s", PQdb(*pg_conn));
    return 1;
}

void
close_connection(PGconn** pg_conn, PGresult **pg_res) {
    clearRes(pg_res);
    PQfinish(*pg_conn);
    elog(LOG, "close_connection(): Libpq connection closed");
}

int
exec_query(PGconn** pg_conn, char *query, PGresult** res) {
    ExecStatusType resStatus;
    elog(LOG, "exec_query(): Start execution query: %s", query);
    *res = PQexec(*pg_conn, query);
    resStatus = PQresultStatus(*res);
    // convert status to string and log
    elog(LOG, "exec_query(): Finish execution query with status: %s", PQresStatus(resStatus));

    //PGRES_COMMAN_OK - Successful completion of a command returning no data
    //PGRES_TUPLES_OK - Successful completion of a command returning data (such as a SELECT or SHOW)
    if (resStatus != PGRES_TUPLES_OK && resStatus != PGRES_COMMAND_OK) {
        elog(ERROR, "exec_query(): Error while executing the query: %s", PQerrorMessage(*pg_conn));
        clearRes(res);
        return 0;
    }
    return 1;
}


char *handle_query(PGresult** res, int *server_error) {
    int rows;
    int cols;
    char jsonResponse[RESPONSE_LENGTH];
    
    char *httpResponse = (char *)malloc(RESPONSE_LENGTH);
    if (httpResponse == NULL) {
        elog(LOG, "handle_query(): httpResponse malloc failed\n");
        *server_error = 1;
        return NULL;
    }

    rows = PQntuples(*res);
    cols = PQnfields(*res);

    elog(LOG, "handle_query(): Number of rows: %d\n", rows);
    elog(LOG, "handle_query(): Number of columns: %d\n", cols);

    // Create JSON response string
    snprintf(jsonResponse, RESPONSE_LENGTH, "{ \"data\": { ");

    // Add column name to response
    for (int i = 0; i < cols; i++) {
        snprintf(jsonResponse, RESPONSE_LENGTH, "%s\"%s\": ", jsonResponse, PQfname(*res, i));
    }

    // Add json data to response
    snprintf(jsonResponse, RESPONSE_LENGTH, "%s%s", jsonResponse, PQgetvalue(*res, 0, 0));

    snprintf(jsonResponse, RESPONSE_LENGTH, "%s } }", jsonResponse);

    elog(LOG, "handle_query(): jsonResponse: %s\n", jsonResponse);

    // Form HTTP response
    if (*res == NULL) {
        snprintf(httpResponse, RESPONSE_LENGTH, "%s", jsonResponse);
    } else {
        elog(LOG, "handle_query(): Received NULL PGresult.");
        snprintf(httpResponse, RESPONSE_LENGTH, "%s", jsonResponse);
    }
    
    elog(LOG, "handle_query(): HTTP Response: %s", httpResponse);
    return httpResponse;
}