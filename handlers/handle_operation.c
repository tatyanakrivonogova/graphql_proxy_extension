#include "handlers.h"
#include "../hashmap/map.h"
#include "../json_graphql/cJSON.h"
#include "../schema/schema_converting.h"
#include "argument_value.h"

#include "postgres.h"

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
        if (prefix_length + strlen(value) + strlen(pos + 2) < buffer_size) {
            strncpy(buffer, format, prefix_length);
            buffer[prefix_length] = '\0';

            strcat(buffer, value);
            strcat(buffer, pos + 2);
        }
    } else {
        // copy original string if %s is not in buffer
        strncpy(buffer, format, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
}

void free_arg_values(ArgValues *argValues) {
    if (argValues != NULL) {
        for (size_t i = 0; i < argValues->argNumber; ++i) {
            free(argValues->argValues[i]);
        }
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
        insert_int(query, MAX_QUERY_LENGTH, format_query, value);
    } else if ((strcmp(type, "TEXT") == 0) || (strcmp(type, "BOOLEAN"))) {
        insert_string(query, MAX_QUERY_LENGTH, format_query, value);
    } else {
        elog(LOG, "Unknown type: %s\n", type);
    }
}

char *set_arguments_to_query(hashmap *resolvers, char *operation_name, ArgValues *argValues) {
    uintptr_t res;
    char *format_query;
    char *query;
    size_t argumentsNumber;
    Argument **arguments;
    
    // get sql-code
    if (hashmap_get(resolvers, operation_name, strlen(operation_name), &res)) {
        elog(LOG, "sql format_query for %s:\n\t\t%s\n", operation_name, ((Operation *)res)->operationSql);
        // copy original format_query
        format_query = (char *)malloc(MAX_QUERY_LENGTH);
        strcpy(format_query, ((Operation *)res)->operationSql);
        // query for setting argument's values
        query = (char *)malloc(MAX_QUERY_LENGTH);
    } else {
        elog(LOG, "sql format_query for %s not found.\n", operation_name);
        return NULL;
    }

    argumentsNumber = ((Operation *)res)->argumentsNumber;
    arguments = ((Operation *)res)->arguments;

    for (size_t i = 0; i < argumentsNumber; ++i) {
        Argument *currentArg = arguments[i];
        char * currentArgValue = find_arg_value(argValues, arguments[i]->argName, arguments[i]->argType);
        if (currentArgValue == NULL) {
            if (strlen(currentArg->defaultValue) != 0) {
                // set default value
                elog(LOG, "set default value\n");
                set_value(format_query, query, currentArg->defaultValue, currentArg->argType);
            } else if (currentArg->nonNullType) {
                elog(LOG, "NULL value in non-null argument\n");
                goto set_arguments_to_query_fail;
            } else {
                // set NULL value
                elog(LOG, "set NULL value\n");
                set_value(format_query, query, "NULL", currentArg->argType);
            }
        } else {
            // set specified value
            elog(LOG, "set specified value\n");
            set_value(format_query, query, currentArgValue, currentArg->argType);
        }
        swap(&format_query, &query);
    }
    swap(&format_query, &query);
    return query;

set_arguments_to_query_fail:
    free(query);
    free(format_query);
    return NULL;
}

void handle_mutation(const char *json_query, hashmap *resolvers) {
    cJSON *json;
    cJSON *definitions;
    char *query_for_execution;

    json = cJSON_Parse(json_query);
    if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "Error: %s %ld\n", error_ptr, error_ptr - json_query);
		}
		cJSON_Delete(json);
		return;
	}

    // access the JSON data
	definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    elog(LOG, "definitions: %p\n", definitions);

    for (int i = 0; i < cJSON_GetArraySize(definitions); ++i)
    {
        cJSON *definition;
        cJSON *definition_kind;
        cJSON *definition_operation;
        cJSON *definition_selection_set;
        cJSON *selections;
        
        definition = cJSON_GetArrayItem(definitions, i);
        definition_kind = cJSON_GetObjectItemCaseSensitive(definition, "kind");
        if (definition_kind != NULL && (cJSON_IsString(definition_kind)) && (definition_kind->valuestring != NULL)
                && (strcmp(definition_kind->valuestring, "OperationDefinition") != 0)) {
            elog(LOG, "Wrong query type\n");
            continue;
        }

        definition_operation = cJSON_GetObjectItemCaseSensitive(definition, "operation");
        if (definition_operation != NULL && (cJSON_IsString(definition_operation)) && (definition_operation->valuestring != NULL)
                && (strcmp(definition_operation->valuestring, "mutation") != 0) && (strcmp(definition_operation->valuestring, "query") != 0)) {
            elog(LOG, "Operation query or mutation expected\n");
            continue;
        }

        definition_selection_set = cJSON_GetObjectItemCaseSensitive(definition, "selectionSet");
        selections = cJSON_GetObjectItemCaseSensitive(definition_selection_set, "selections");

        for (int j = 0; j < cJSON_GetArraySize(selections); ++j)
        {
            cJSON *selection;
            cJSON *selection_name;
            cJSON *selection_name_value;
            cJSON *selection_arguments;
            ArgValues argValues;

            selection = cJSON_GetArrayItem(selections, j);
            selection_name = cJSON_GetObjectItemCaseSensitive(selection, "name");
            selection_name_value = cJSON_GetObjectItemCaseSensitive(selection_name, "value");
            if (selection_name_value != NULL && (cJSON_IsString(selection_name_value)) 
                    && (selection_name_value->valuestring != NULL))
                elog(LOG, "operation %s is called\n", selection_name_value->valuestring);

            selection_arguments = cJSON_GetObjectItemCaseSensitive(selection, "arguments");
            argValues.argNumber = cJSON_GetArraySize(selection_arguments);
            for (int k = 0; k < cJSON_GetArraySize(selection_arguments); ++k)
            {
                cJSON *argument;
                cJSON *argument_name;
                cJSON *argument_name_value;
                cJSON *argument_value;
                cJSON *argument_value_kind;
                cJSON *argument_value_value;

                ArgValue *currentArgValue = (ArgValue *)malloc(sizeof(ArgValue));
                if (currentArgValue == NULL) {
                    elog(LOG, "Argument value malloc failed\n");
                    free_arg_values(&argValues);
                    goto handle_operation_fail;
                }

                // parse argument name
                argument = cJSON_GetArrayItem(selection_arguments, k);
                argument_name = cJSON_GetObjectItemCaseSensitive(argument, "name");
                argument_name_value = cJSON_GetObjectItemCaseSensitive(argument_name, "value");
                if (argument_name_value != NULL && (cJSON_IsString(argument_name_value)) 
                        && (argument_name_value->valuestring != NULL)) {
                    elog(LOG, "argument[%d] name: %s\n", k, argument_name_value->valuestring);
                    strcpy(currentArgValue->argName, argument_name_value->valuestring);
                }

                // parse argument type
                argument_value = cJSON_GetObjectItemCaseSensitive(argument, "value");
                argument_value_kind = cJSON_GetObjectItemCaseSensitive(argument_value, "kind");
                if (argument_value_kind != NULL && (cJSON_IsString(argument_value_kind)) 
                    && (argument_value_kind->valuestring != NULL)) {
                    elog(LOG, "argument[%d] kind: %s\n", k, argument_value_kind->valuestring);
                    strcpy(currentArgValue->argType, argument_value_kind->valuestring);

                    // parse argument value
                    argument_value_value = cJSON_GetObjectItemCaseSensitive(argument_value, "value");
                    if (argument_value_value == NULL) {
                        elog(LOG, "Value of argument is not defined\n");
                        currentArgValue->isNull = true;
                        continue;
                    }

                    // set value of argument
                    currentArgValue->isNull = false;
                    strcpy(currentArgValue->value, argument_value_value->valuestring);
                }
                argValues.argValues[k] = currentArgValue;
            }

            if ((query_for_execution = set_arguments_to_query(resolvers, selection_name_value->valuestring, &argValues)) == NULL) {
                elog(LOG, "Set arguments to query failed\n");
                goto handle_operation_fail;
            } else {
                elog(LOG, "Set arguments to query successfully\n");
                elog(LOG, "Query for execution: %s\n", query_for_execution);
                return;
            }
        }
        // execute query
        // TO DO
handle_operation_fail:
        elog(LOG, "handle_operation_fail\n");
    }
}