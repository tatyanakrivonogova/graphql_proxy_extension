#include "handle_operation.h"

#include "../json_graphql/cJSON.h"
#include "../io_uring/event_handling.h"
#include "../postgres_connect/postgres_connect.h"
#include "../io_uring/multiple_user_access.h"
#include "../schema/schema.h"
#include "defines.h"

#include "postgres.h"

void free_arg_values(ArgValues *argValues) {
    if (argValues != NULL) {
        for (size_t i = 0; i < argValues->argNumber; ++i) {
            free(argValues->argValues[i]);
        }
    }
}

char *set_arguments_to_query(hashmap *resolvers, char *operation_name, ArgValues *argValues, int *server_error) {
    uintptr_t res;
    char *format_query;
    char *query;
    size_t argumentsNumber;
    Argument **arguments;
    
    // get sql-code
    if (hashmap_get(resolvers, operation_name, strlen(operation_name), &res)) {
        elog(LOG, "set_arguments_to_query(): sql format_query for %s:\n\t\t%s\n", operation_name, ((Operation *)res)->operationSql);
        // copy original format_query
        format_query = (char *)malloc(QUERY_LENGTH);
        strcpy(format_query, ((Operation *)res)->operationSql);
        // query for setting argument's values
        query = (char *)malloc(QUERY_LENGTH);
    } else {
        elog(LOG, "set_arguments_to_query(): sql format_query for %s not found.\n", operation_name);
        *server_error = 1;
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
                elog(LOG, "set_arguments_to_query(): set default value\n");
                set_value(format_query, query, currentArg->defaultValue, currentArg->argType);
            } else if (currentArg->nonNullType) {
                elog(LOG, "set_arguments_to_query(): NULL value in non-null argument\n");
                goto set_arguments_to_query_fail;
            } else {
                // set NULL value
                elog(LOG, "set_arguments_to_query(): set NULL value\n");
                set_value(format_query, query, "NULL", currentArg->argType);
            }
        } else {
            // set specified value
            elog(LOG, "set_arguments_to_query(): set specified value\n");
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

char *handle_operation(const char *json_query, hashmap *resolvers, int fd, int *server_error) {
    cJSON *json;
    cJSON *definitions;
    char *query_for_execution;

    cJSON *definition;
    cJSON *definition_kind;
    cJSON *definition_operation;
    cJSON *definition_selection_set;
    cJSON *selections;
    cJSON *selection;
    cJSON *selection_name;
    cJSON *selection_name_value;
    cJSON *selection_arguments;
    ArgValues argValues;

    json = cJSON_Parse(json_query);
    if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "handle_operation(): Parsing json failed\nError: %s %ld\n", error_ptr, error_ptr - json_query);
		}
		cJSON_Delete(json);
		return NULL;
	}

    // access the JSON data
	definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    if (cJSON_GetArraySize(definitions) != 1) {
        elog(LOG, "handle_operation(): Request should consist one operation\n");
        return NULL;
    }
    
    definition = cJSON_GetArrayItem(definitions, 0);
    definition_kind = cJSON_GetObjectItemCaseSensitive(definition, "kind");
    if (definition_kind != NULL && (cJSON_IsString(definition_kind)) && (definition_kind->valuestring != NULL)
            && (strcmp(definition_kind->valuestring, "OperationDefinition") != 0)) {
        elog(LOG, "handle_operation(): Wrong query kind\n");
    }

    definition_operation = cJSON_GetObjectItemCaseSensitive(definition, "operation");
    if (definition_operation != NULL && (cJSON_IsString(definition_operation)) && (definition_operation->valuestring != NULL)
            && (strcmp(definition_operation->valuestring, "mutation") != 0) && (strcmp(definition_operation->valuestring, "query") != 0)) {
        elog(LOG, "handle_operation(): Operation query or mutation expected\n");
    }

    definition_selection_set = cJSON_GetObjectItemCaseSensitive(definition, "selectionSet");
    selections = cJSON_GetObjectItemCaseSensitive(definition_selection_set, "selections");

    if (cJSON_GetArraySize(selections) != 1) {
        elog(LOG, "handle_operation(): Only one operation may be called\n");
        return NULL;
    }

    selection = cJSON_GetArrayItem(selections, 0);
    selection_name = cJSON_GetObjectItemCaseSensitive(selection, "name");
    selection_name_value = cJSON_GetObjectItemCaseSensitive(selection_name, "value");
    if (selection_name_value != NULL && (cJSON_IsString(selection_name_value)) 
            && (selection_name_value->valuestring != NULL))
        elog(LOG, "handle_operation(): operation %s is called\n", selection_name_value->valuestring);

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
            elog(LOG, "handle_operation(): Argument value malloc failed\n");
            free_arg_values(&argValues);
            goto handle_operation_fail;
        }

        // parse argument name
        argument = cJSON_GetArrayItem(selection_arguments, k);
        argument_name = cJSON_GetObjectItemCaseSensitive(argument, "name");
        argument_name_value = cJSON_GetObjectItemCaseSensitive(argument_name, "value");
        if (argument_name_value != NULL && (cJSON_IsString(argument_name_value)) 
                && (argument_name_value->valuestring != NULL)) {
            elog(LOG, "handle_operation(): argument[%d] name: %s\n", k, argument_name_value->valuestring);
            strcpy(currentArgValue->argName, argument_name_value->valuestring);
        }

        // parse argument type
        argument_value = cJSON_GetObjectItemCaseSensitive(argument, "value");
        argument_value_kind = cJSON_GetObjectItemCaseSensitive(argument_value, "kind");
        if (argument_value_kind != NULL && (cJSON_IsString(argument_value_kind)) 
            && (argument_value_kind->valuestring != NULL)) {
            elog(LOG, "handle_operation(): argument[%d] kind: %s\n", k, argument_value_kind->valuestring);
            strcpy(currentArgValue->argType, argument_value_kind->valuestring);

            // parse argument value
            argument_value_value = cJSON_GetObjectItemCaseSensitive(argument_value, "value");
            if (argument_value_value == NULL) {
                elog(LOG, "handle_operation(): Value of argument is not defined\n");
                currentArgValue->isNull = true;
                continue;
            }

            // set value of argument
            currentArgValue->isNull = false;
            strcpy(currentArgValue->value, argument_value_value->valuestring);
        }
        argValues.argValues[k] = currentArgValue;
    }

    if ((query_for_execution = set_arguments_to_query(resolvers, selection_name_value->valuestring, &argValues, server_error)) == NULL) {
        elog(LOG, "handle_operation(): Set arguments to query failed\n");
        goto handle_operation_fail;
    } else {
        int index;
        char *response = NULL;
        elog(LOG, "handle_operation(): Set arguments to query successfully\n");
        elog(LOG, "handle_operation(): Query for execution: %s\n", query_for_execution);

        // execute query
        if (get_conn_index(fd, &index)) {
            elog(LOG, "handle_operation(): execution...\n");
            if (exec_query(&conns[index].pg_conn, query_for_execution, &conns[index].pg_res)) {
                response = handle_query(&conns[index].pg_res, server_error);
            } else {
                elog(LOG, "handle_operation(): Query execution failed.");
                *server_error = 1;
            }
        } else {
            elog(LOG, "handle_operation(): get_conn_index fail\n");
            *server_error = 1;
        }

        free_arg_values(&argValues);
        return response;
    }
    
handle_operation_fail:
    elog(LOG, "handle_operation(): handle_operation_fail\n");
    return NULL;
}