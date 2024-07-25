#include "postgres_connect.h"

#include "postgres.h"

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

    // char* query = "SELECT * FROM table1;";
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

    //clear used resources
    close_connection(&pg_conn, &res);
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
    PQclear(*pg_res);
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
        PQclear(*res);
        return 0;
    }
    return 1;
}

// void handle_query(PGresult* res) {
//     int rows, cols;

//     rows = PQntuples(res);
//     cols = PQnfields(res);

//     elog(LOG, "Number of rows: %d\n", rows);
//     elog(LOG, "Number of columns: %d\n", cols);
//     // Print the column names
//     for (int i = 0; i < cols; i++) {
//         elog(LOG, "%s\t", PQfname(res, i));
//     }

//     // Print all the rows and columns
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//         // Print the column value
//             elog(LOG, "%s\t", PQgetvalue(res, i, j));
//         }
//         elog(LOG, "-------------------------------------------------------");
//     }
// }

void handle_query(PGresult* res) {
    if (res == NULL) {
        elog(ERROR, "Received NULL PGresult.");
        return;
    }

    int rows = PQntuples(res);
    int cols = PQnfields(res);

    elog(LOG, "Number of rows: %d\n", rows);
    elog(LOG, "Number of columns: %d\n", cols);

    // Print the column names
    for (int i = 0; i < cols; i++) {
        elog(LOG, "%s\t", PQfname(res, i));
    }

    // Print all the rows and columns
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            elog(LOG, "%s\t", PQgetvalue(res, i, j));
        }
        elog(LOG, "-------------------------------------------------------");
    }
}