#include "postgres_connect.h"

#include "postgres.h"

void 
test_connect(void) {
    char *query = "INSERT INTO table1 values(500);";
    char *conn_info = "dbname=postgres host=localhost port=5432";
    int rows, cols;
    PGconn *conn;
    PGresult *res;
    if (!create_connection(&conn, conn_info)) {
        return;
    }

    // char* query = "SELECT * FROM table1;";
    //it is possible to exec many commands like "INSERT INTO table1 values(5); SELECT * FROM table1;"
    exec_query(&conn, "SELECT * FROM table1;", &res);
    exec_query(&conn, query, &res);
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

    //clear used resources
    PQclear(res);
    close_connection(&conn);
}

int 
create_connection(PGconn** conn, char* conn_info) {
    *conn = PQconnectdb(conn_info);

    if (PQstatus(*conn) != CONNECTION_OK) {
        elog(ERROR, "Error while connecting to the database server: %s\n", PQerrorMessage(*conn));
        PQfinish(*conn);
        return 0;
    }
    // We have successfully established a connection to the database server
    elog(LOG, "Connection Established");
    elog(LOG, "Port: %s", PQport(*conn));
    elog(LOG, "Host: %s", PQhost(*conn));
    elog(LOG, "DBName: %s", PQdb(*conn));
    return 1;
}

void
close_connection(PGconn** conn) {
    PQfinish(*conn);
    elog(LOG, "Libpq connection closed");
}

int
exec_query(PGconn** conn, char *query, PGresult** res) {
    ExecStatusType resStatus;
    elog(LOG, "Start execution query: %s", query);
    *res = PQexec(*conn, query);
    resStatus = PQresultStatus(*res);
    // convert status to string and log
    elog(LOG, "Finish execution query with status: %s", PQresStatus(resStatus));

    //PGRES_COMMAN_OK - Successful completion of a command returning no data
    //PGRES_TUPLES_OK - Successful completion of a command returning data (such as a SELECT or SHOW)
    if (resStatus != PGRES_TUPLES_OK && resStatus != PGRES_COMMAND_OK) {
        elog(ERROR, "Error while executing the query: %s", PQerrorMessage(*conn));
        PQclear(*res);
        return 0;
    }
    return 1;
}