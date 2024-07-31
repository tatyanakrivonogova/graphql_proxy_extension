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

char **get_param_values(hashmap *resolvers, char *operation_name, ArgValues *argValues, 
        size_t *param_number, int *server_error) {
    char **param_values;
    uintptr_t res;
    size_t argumentsNumber;
    Argument **arguments;
    
    // get sql-code
    if (hashmap_get(resolvers, operation_name, strlen(operation_name), &res)) {
        elog(LOG, "get_param_values(): query %s:\n", operation_name);
    } else {
        elog(LOG, "set_arguments_to_query(): sql format_query for %s not found.\n", operation_name);
        *server_error = 1;
        return NULL;
    }

    argumentsNumber = ((Operation *)res)->argumentsNumber;
    *param_number = argumentsNumber;
    arguments = ((Operation *)res)->arguments;

    param_values = (char **)malloc(sizeof(char *) * argumentsNumber);
    if (param_values == NULL) {
        elog(LOG, "get_param_values(): Param values malloc failed\n");
        *server_error = 1;
        return NULL;
    }

    for (size_t i = 0; i < argumentsNumber; ++i) {
        Argument *currentArg = arguments[i];
        char * currentArgValue = find_arg_value(argValues, arguments[i]->argName, arguments[i]->argType);
        if (currentArgValue == NULL) {
            if (strlen(currentArg->defaultValue) != 0) {
                // set default value
                elog(LOG, "get_param_values(): set default value\n");
                param_values[i] = currentArg->defaultValue;
            } else if (currentArg->nonNullType) {
                elog(LOG, "get_param_values(): NULL value in non-null argument\n");
                goto set_arguments_to_query_fail;
            } else {
                // set NULL value
                elog(LOG, "get_param_values(): set NULL value\n");
                param_values[i] = "NULL"; //????
            }
        } else {
            // set specified value
            elog(LOG, "get_param_values(): set specified value\n");
            param_values[i] = currentArgValue;
        }
        elog(LOG, "get_param_values(): param_values[%ld] = %s\n", i, param_values[i]);
    }

    return param_values;

set_arguments_to_query_fail:
    free(param_values);
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
    size_t param_number;
    char **param_values;

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

    // get param values for prepared statement
    param_number = 0;
    param_values = get_param_values(resolvers, selection_name_value->valuestring, &argValues, &param_number, server_error);
    if (param_values == NULL) {
        free_arg_values(&argValues);
        goto handle_operation_fail;
    }

    char *response = NULL;

    // execute query
    PGresult *result = execute_prepared_statement(pg_conn, selection_name_value->valuestring, 
            (const char **)param_values, param_number);
    if (result) {
        response = handle_query(&result, server_error);
    }

    free_arg_values(&argValues);
    free(param_values);
    return response;
    
handle_operation_fail:
    elog(LOG, "handle_operation(): handle_operation_fail\n");
    return NULL;
}