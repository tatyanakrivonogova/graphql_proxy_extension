#include "postgres.h"


Datum my_run(PG_FUNCTION_AGRS);

char* to_sql(char *expr);

char* handle_operation_query(const char *json_query, int fd);