#include "resolverLoader.h"

#include "postgres.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* load_function_body(const char* function_name) {
    char line[MAX_FUNCTION_SIZE];
    char *function_body = NULL;
    size_t function_body_size = 0;

    FILE *file = fopen("../contrib/graphql_proxy/json_graphql/resolvers/functions.sql", "r");
    if (file == NULL) {
        elog(LOG, "Failed to open file");
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        char *function_start = strstr(line, function_name);
        if (function_start != NULL) {
            char *function_end;

            function_start += strlen(function_name) + 2; // Skip FunctionName='
            function_end = strchr(function_start, '\'');
            if (function_end != NULL) {
                size_t body_len = function_end - function_start;
                function_body = (char*)realloc(function_body, function_body_size + body_len + 1);
                strncpy(function_body + function_body_size, function_start, body_len);
                function_body_size += body_len;
                function_body[function_body_size] = '\0';
            }
        }
    }

    fclose(file);
    return function_body;
}

// int main() {
//     char* function_name = "Function1";
//     char* function_body = load_function_body(function_name);

//     if (function_body != NULL) {
//         elog(LOG, "Function body for %s:\n%s\n", function_name, function_body);
//         free(function_body);
//     } else {
//         elog(LOG, "Function %s not found.\n", function_name);
//     }

//     return 0;
// }
