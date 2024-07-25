#include "../hashmap/map.h"
#include "argument_value.h"

#define MAX_QUERY_LENGTH 256

void handle_mutation(const char *json_query, hashmap *resolvers);

void free_arg_values(ArgValues *argValues);
char *set_arguments_to_query(hashmap *resolvers, char *operation_name, ArgValues *argValues);