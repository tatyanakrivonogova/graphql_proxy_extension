#include <stddef.h>
#include <stdbool.h>

#include "schema.h"
#include "../hashmap/map.h"

#define QUERY_LENGTH 256
#define ALTER_QUERIES_NUMBER 10

bool is_type_exists(char* type_name);

void create_foreign_key(char* sql_alter, char* table_name, char* another_table_name, char* field_name);

void free_alter_queries(char** sql_alter_queries, size_t sql_alter_queries_num);

void free_arguments(Operation *mutation);

hashmap *schema_convert(const char *json_schema);

// int create_connection(PGconn** conn, char* conn_info);

// void close_connection(PGconn** conn);

// int exec_query(PGconn** conn, char *query, PGresult** res);

// void handle_query(PGresult* res);