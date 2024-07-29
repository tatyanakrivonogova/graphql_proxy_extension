#include "query_parser.h"

#include "../json_graphql/cJSON.h"
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

char* handle_operation_query(const char *json_query, int fd)
{
    char* response; 
    cJSON *json;
    cJSON *definitions;
    char *query_for_execution;
    json = cJSON_Parse(json_query);
    // elog(LOG, "json: %s", json->string);
    if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "Error: %s %ld\n", error_ptr, error_ptr - json_query);
		}
		cJSON_Delete(json);
		return NULL;
	}

    // access the JSON data
	definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    elog(LOG, "definitions: %p\n", definitions);
    if (cJSON_GetArraySize(definitions) != 1) {
        elog(LOG, "Request should consist one operation\n");
        return NULL;
    }

    return response;
}
