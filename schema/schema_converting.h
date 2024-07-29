#include <stddef.h>
#include <stdbool.h>

#include "schema.h"
#include "../hashmap/map.h"

#define ALTER_QUERIES_NUMBER 10

bool is_type_exists(char* type_name);

void create_foreign_key(char* sql_alter, char* table_name, char* another_table_name, char* field_name);

void free_alter_queries(char** sql_alter_queries, size_t *sql_alter_queries_num);

void free_arguments(Operation *mutation);

hashmap *schema_convert(const char *json_schema, const char* file_types_reflection, 
        char *db_name, char *db_host, int db_port, char *sql_filename);