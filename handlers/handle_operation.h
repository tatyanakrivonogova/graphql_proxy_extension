#include "utils.h"
#include "../hashmap/map.h"

void 
free_arg_values(ArgValues *argValues);

char *
set_arguments_to_query(hashmap *resolvers, char *operation_name, ArgValues *argValues);

char *
handle_operation(const char *json_query, hashmap *resolvers, int fd);