#include <stdbool.h>
#include "stddef.h"

#define NAME_LENGTH 63
#define MAX_TYPES_NUMBER 20
#define MAX_QUERIES_NUMBER 20
#define MAX_MUTATIONS_NUMBER 20
#define MAX_ARGUMENTS_NUMBER 20

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


// mutation with sql-function and argument types
typedef struct {
    size_t numCreatedMutations;
    // char createdMutations[MAX_MUTATIONS_NUMBER][NAME_LENGTH];
    Mutation *createdMutations[MAX_MUTATIONS_NUMBER];
} Mutations;

// query with sql-
typedef struct {
    size_t numCreatedQueries;
    char createdQueries[MAX_QUERIES_NUMBER][NAME_LENGTH];
} Queries;

void free_arguments(Mutation *mutation);