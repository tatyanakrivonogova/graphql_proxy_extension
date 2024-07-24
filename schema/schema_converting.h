#include <stddef.h>
#include <stdbool.h>

#include "../hashmap/map.h"

#define NAME_LENGTH 63
#define MAX_TYPES_NUMBER 20
#define MAX_QUERIES_NUMBER 20
#define MAX_MUTATIONS_NUMBER 20
#define MAX_ARGUMENTS_NUMBER 20
#define QUERY_LENGTH 256
#define ALTER_QUERIES_NUMBER 10


typedef struct {
    char argName[NAME_LENGTH];
    char argType[NAME_LENGTH];
    bool nonNullType;
} Argument;
typedef struct {
    char mutationName[NAME_LENGTH];
    char *mutationSql;
    size_t argumentsNumber;
    Argument *arguments[MAX_ARGUMENTS_NUMBER];
    char return_value[NAME_LENGTH];
} Mutation;


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
    // char createdMutations[MAX_MUTATIONS_NUMBER][NAME_LENGTH];
    Mutation *createdMutations[MAX_MUTATIONS_NUMBER];
} Mutations;
// Mutations mutations;


bool is_type_exists(char* type_name);

void create_foreign_key(char* sql_alter, char* table_name, char* another_table_name, char* field_name);

void free_alter_queries(char** sql_alter_queries, size_t sql_alter_queries_num);

void free_arguments(Mutation *mutation);

hashmap *schema_convert(const char *json_schema);

// int create_connection(PGconn** conn, char* conn_info);

// void close_connection(PGconn** conn);

// int exec_query(PGconn** conn, char *query, PGresult** res);

// void handle_query(PGresult* res);