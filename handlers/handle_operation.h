#include "utils.h"
#include "../hashmap/map.h"
#include "../postgres_connect/postgres_connect.h"

extern PGconn *pg_conn;

void 
free_arg_values(ArgValues *argValues);

char **
get_param_values(hashmap *resolvers, char *operation_name, ArgValues *argValues, 
        size_t *param_number, int *server_error);

char *
set_arguments_to_query(hashmap *resolvers, char *operation_name, ArgValues *argValues, int *server_error);

char *
handle_operation(const char *json_query, hashmap *resolvers, int fd, int *server_error);