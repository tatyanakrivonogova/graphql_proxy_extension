#include "utils.h"
#include "defines.h"

#include "postgres.h"
#include <string.h>

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

void insert_int(char *buffer, size_t buffer_size, const char *format, const char *value) {
    // find %d
    const char *pos = strstr(format, "%d");
    if (pos != NULL) {
        // copy string before %d
        size_t prefix_length = pos - format;
        if (prefix_length + strlen(value) + strlen(pos + 2) < buffer_size) {
            strncpy(buffer, format, prefix_length);
            buffer[prefix_length] = '\0';

            strcat(buffer, value);
            strcat(buffer, pos + 2);
        }
    } else {
        // copy original string if %d is not in buffer
        strncpy(buffer, format, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
}


void insert_string(char *buffer, size_t buffer_size, const char *format, const char *value) {
    // find %s
    const char *pos = strstr(format, "%s");
    if (pos != NULL) {
        // copy string before %s
        size_t prefix_length = pos - format;
        if (prefix_length + 2 + strlen(value) + strlen(pos + 2) < buffer_size) {
            strncpy(buffer, format, prefix_length);
            buffer[prefix_length] = '\0';

            strcat(buffer, "\'");
            strcat(buffer, value);
            strcat(buffer, "\'");
            strcat(buffer, pos + 2);
        }
    } else {
        // copy original string if %s is not in buffer
        strncpy(buffer, format, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
}

int is_same(char *pg_type, char *json_type) {
    if ((strcmp(pg_type, "INT") == 0) && (strcmp(json_type, "IntValue") == 0)) return 0;
    if ((strcmp(pg_type, "UUID") == 0) && (strcmp(json_type, "IntValue") == 0)) return 0;
    if ((strcmp(pg_type, "TEXT") == 0) && (strcmp(json_type, "StringValue") == 0)) return 0;
    // add boolean and other
    return 1;
}

char *find_arg_value(ArgValues *argValues, char *argName, char *argType) {
    for (size_t i = 0; i < argValues->argNumber; ++i) {
        if (argValues->argValues[i] != NULL 
                && (strcmp(argValues->argValues[i]->argName, argName) == 0)
                && (is_same(argType, argValues->argValues[i]->argType) == 0)) {
            if (argValues->argValues[i]->isNull) {
                elog(LOG, "is null\n");
                return NULL;
            } else {
                elog(LOG, "found: %s\n", argValues->argValues[i]->value);
                return argValues->argValues[i]->value;
            }
        }
    }
    return NULL;
}

void set_value(char *format_query, char *query, char *value, char *type) {
    if ((strcmp(type, "UUID") == 0) || (strcmp(type, "INT") == 0)) {
        insert_int(query, QUERY_LENGTH, format_query, value);
    } else if ((strcmp(type, "TEXT") == 0) || (strcmp(type, "BOOLEAN"))) {
        insert_string(query, QUERY_LENGTH, format_query, value);
    } else {
        elog(LOG, "Unknown type: %s\n", type);
    }
}