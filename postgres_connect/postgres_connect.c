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
    //PQclear(*pg_res);
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
        // PQclear(*res);
        clearRes(res);
        return 0;
    }
    return 1;
}

// void handle_query(PGresult** res) {
//     int rows;
//     int cols;
//     if (*res == NULL) {
//         elog(ERROR, "Received NULL PGresult.");
//         return;
//     }

//     rows = PQntuples(*res);
//     cols = PQnfields(*res);

//     elog(LOG, "Number of rows: %d\n", rows);
//     elog(LOG, "Number of columns: %d\n", cols);

//     // Print the column names
//     for (int i = 0; i < cols; i++) {
//         elog(LOG, "%s\t", PQfname(*res, i));
//     }

//     // Print all the rows and columns
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//             elog(LOG, "%s\t", PQgetvalue(*res, i, j));
//         }
//         elog(LOG, "-------------------------------------------------------");
//     }

//     // PQclear(res);
//     clearRes(res);
// }


void handle_query(PGresult** res) {
    int rows;
    int cols;
    // if (*res == NULL) {
    //     elog(ERROR, "Received NULL PGresult.");
    //     return;
    // }

    rows = PQntuples(*res);
    cols = PQnfields(*res);

    elog(LOG, "Number of rows: %d\n", rows);
    elog(LOG, "Number of columns: %d\n", cols);

    // Create JSON response string
    char jsonResponse[RESPONSE_LENGTH];
    snprintf(jsonResponse, sizeof(jsonResponse), "{ \"data\": { ");

    // Add column name to response
    for (int i = 0; i < cols; i++) {
        snprintf(jsonResponse, sizeof(jsonResponse), "%s\"%s\": ", jsonResponse, PQfname(*res, i));
    }

    // Add json data to response
    snprintf(jsonResponse, sizeof(jsonResponse), "%s%s", jsonResponse, PQgetvalue(*res, 0, 0));

    // // Add data to response
    // for (int i = 0; i < rows; i++) {
    //     snprintf(jsonResponse, sizeof(jsonResponse), "%s[ ", jsonResponse);
    //     for (int j = 0; j < cols; j++) {
    //         snprintf(jsonResponse, sizeof(jsonResponse), "%s\"%s\", ", jsonResponse, PQgetvalue(*res, i, j));
    //     }
    //     snprintf(jsonResponse, sizeof(jsonResponse), "%s ], ", jsonResponse);
    // }

    snprintf(jsonResponse, sizeof(jsonResponse), "%s } }", jsonResponse);

    // Form HTTP response
    char httpResponse[RESPONSE_LENGTH];
    if (*res == NULL) {
        snprintf(httpResponse, sizeof(httpResponse), "HTTP/1.1 500 Internal Server Error\r\nContent-Type: application/json\r\n\r\n%s", jsonResponse);
    } else {
        elog(LOG, "Received NULL PGresult.");
        snprintf(httpResponse, sizeof(httpResponse), "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n%s", jsonResponse);
    }
    
    elog(LOG, "HTTP Response: %s", httpResponse);
    // Send HTTP response here

}