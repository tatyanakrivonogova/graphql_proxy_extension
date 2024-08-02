#include <stdbool.h>
#include "stddef.h"

#include "../postgres_connect/postgres_connect.h"

#define NAME_LENGTH 63
#define TYPE_LENGTH 63
#define DEFAULT_VALUE_LENGTH 63
#define MAX_TYPES_NUMBER 20
#define MAX_QUERIES_NUMBER 20
#define MAX_MUTATIONS_NUMBER 20
#define MAX_ARGUMENTS_NUMBER 20

typedef struct {
    char argName[NAME_LENGTH];
    char argType[TYPE_LENGTH];
    bool nonNullType;
    char defaultValue[DEFAULT_VALUE_LENGTH];
} Argument;


// operation = named query or mutation (which is called by its name and arguments)
typedef struct {
    char operationName[NAME_LENGTH];
    // char *operationSql;
    PGresult *prepared_stmt;
    size_t argumentsNumber;
    Argument *arguments[MAX_ARGUMENTS_NUMBER];
    char return_value[NAME_LENGTH];
} Operation;


// types which are already converted to PostgresQL
typedef struct Types{
    size_t numCreatedTypes;
    char createdTypes[MAX_TYPES_NUMBER][NAME_LENGTH];
} Types;
extern Types types;


// mutation with sql-function and argument types
typedef struct {
    size_t numCreatedMutations;
    Operation *createdMutations[MAX_MUTATIONS_NUMBER];
} Mutations;

// query with sql-
typedef struct {
    size_t numCreatedQueries;
    Operation *createdMutations[MAX_MUTATIONS_NUMBER];
} Queries;

void free_arguments(Operation *operation);