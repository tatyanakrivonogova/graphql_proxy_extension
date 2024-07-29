#include "query_parser.h"
#include <stdio.h>

Datum
my_run(PG_FUNCTION_ARGS)
{
	// char *input = text_to_cstring(PG_GETARG_TEXT_PP(0));
}

char* to_sql(char *expr)
{
    char* result = (char *)malloc(sizeof(char) * 300);
    char* head = (char *)malloc(sizeof(char) * 300);
    sscanf(expr, "%s{", result);
    return result;
}