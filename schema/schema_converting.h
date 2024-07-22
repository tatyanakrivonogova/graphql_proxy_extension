#include <stddef.h>
#include <stdbool.h>

#define NAME_LENGTH 63
#define MAX_TYPES_NUMBER 20
#define MAX_QUERIES_NUMBER 20
#define MAX_MUTATIONS_NUMBER 20
#define QUERY_LENGTH 256
#define ALTER_QUERIES_NUMBER 10


// types which are already converted to PostgresQL
typedef struct {
    size_t numCreatedTypes;
    char createdTypes[MAX_TYPES_NUMBER][NAME_LENGTH];
} Types;
Types types;


typedef struct {
    size_t numCreatedQueries;
    char createdQueries[MAX_QUERIES_NUMBER][NAME_LENGTH];
} Queries;
Queries queries;


typedef struct {
    size_t numCreatedMutations;
    char createdMutations[MAX_MUTATIONS_NUMBER][NAME_LENGTH];
} Mutations;
Mutations mutations;

bool is_type_exists(char* type_name);

void create_foreign_key(char* sql_alter, char* table_name, char* another_table_name, char* field_name);

void free_alter_queries(char** sql_alter_queries, size_t sql_alter_queries_num);

void schema_convert(const char *json_schema);