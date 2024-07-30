#include "argument_value.h"

#include <stddef.h>

void swap(char **a, char **b);

void insert_int(char *buffer, size_t buffer_size, const char *format, const char *value);

void insert_string(char *buffer, size_t buffer_size, const char *format, const char *value);

int is_same(char *pg_type, char *json_type);

char *find_arg_value(struct ArgValues *argValues, char *argName, char *argType);

void set_value(char *format_query, char *query, char *value, char *type);