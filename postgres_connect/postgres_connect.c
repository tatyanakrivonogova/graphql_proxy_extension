#include "postgres_connect.h"

#include "postgres.h"
#include <unistd.h>

void 
test_connect(void) {
    char *query = "INSERT INTO table1 values(500);";
    char *conn_info = "dbname=postgres host=localhost port=5432";
    int rows, cols;
    PGconn *pg_conn;
    PGresult *res;
    if (!create_connection(&pg_conn, conn_info)) {
        return;
    }

    //it is possible to exec many commands like "INSERT INTO table1 values(5); SELECT * FROM table1;"
    exec_query(&pg_conn, "SELECT * FROM table1;", &res);
    exec_query(&pg_conn, query, &res);
    rows = PQntuples(res);
    cols = PQnfields(res);

    elog(LOG, "Number of rows: %d", rows);
    elog(LOG, "Number of columns: %d", cols);
    // Print the column names
    for (int i = 0; i < cols; i++) {
        elog(LOG, "%s\t", PQfname(res, i));
    }

    // Print all the rows and columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
        // Print the column value
            elog(LOG, "%s\t", PQgetvalue(res, i, j));
        }
        elog(LOG, "-------------------------------------------------------");
    }

    // clear used resources
    close_connection(&pg_conn, &res);
}

void
clearRes(PGresult** res) {
    if (*res != NULL) PQclear(*res);
    *res = NULL;
}

int 
create_connection(PGconn** pg_conn, char* conn_info) {
    *pg_conn = PQconnectdb(conn_info);

    if (PQstatus(*pg_conn) != CONNECTION_OK) {
        elog(ERROR, "Error while connecting to the database server: %s\n", PQerrorMessage(*pg_conn));
        PQfinish(*pg_conn);
        return 0;
    }
    // We have successfully established a connection to the database server
    elog(LOG, "\nConnection Established");
    elog(LOG, "Port: %s", PQport(*pg_conn));
    elog(LOG, "Host: %s", PQhost(*pg_conn));
    elog(LOG, "DBName: %s", PQdb(*pg_conn));
    return 1;
}

void
close_connection(PGconn** pg_conn, PGresult **pg_res) {
    clearRes(pg_res);
    PQfinish(*pg_conn);
    elog(LOG, "Libpq connection closed");
}

int
exec_query(PGconn** pg_conn, char *query, PGresult** res) {
    ExecStatusType resStatus;
    elog(LOG, "Start execution query: %s", query);
    *res = PQexec(*pg_conn, query);
    resStatus = PQresultStatus(*res);
    // convert status to string and log
    elog(LOG, "Finish execution query with status: %s", PQresStatus(resStatus));

    //PGRES_COMMAN_OK - Successful completion of a command returning no data
    //PGRES_TUPLES_OK - Successful completion of a command returning data (such as a SELECT or SHOW)
    if (resStatus != PGRES_TUPLES_OK && resStatus != PGRES_COMMAND_OK) {
        elog(ERROR, "Error while executing the query: %s", PQerrorMessage(*pg_conn));
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
        elog(LOG, "httpResponse malloc failed\n");
        *server_error = 1;
        return NULL;
    }

    rows = PQntuples(*res);
    cols = PQnfields(*res);

    elog(LOG, "Number of rows: %d\n", rows);
    elog(LOG, "Number of columns: %d\n", cols);

    // Create JSON response string
    snprintf(jsonResponse, RESPONSE_LENGTH, "{ \"data\": { ");

    // Add column name to response
    for (int i = 0; i < cols; i++) {
        snprintf(jsonResponse, RESPONSE_LENGTH, "%s\"%s\": ", jsonResponse, PQfname(*res, i));
    }

    // Add json data to response
    snprintf(jsonResponse, RESPONSE_LENGTH, "%s%s", jsonResponse, PQgetvalue(*res, 0, 0));

    snprintf(jsonResponse, RESPONSE_LENGTH, "%s } }", jsonResponse);

    elog(LOG, "jsonResponse: %s\n", jsonResponse);

    // Form HTTP response
    if (*res == NULL) {
        snprintf(httpResponse, RESPONSE_LENGTH, "%s", jsonResponse);
    } else {
        elog(LOG, "Received NULL PGresult.");
        snprintf(httpResponse, RESPONSE_LENGTH, "%s", jsonResponse);
    }
    
    elog(LOG, "HTTP Response: %s", httpResponse);
    return httpResponse;
}