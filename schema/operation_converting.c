#include "operation_converting.h"

#include "postgres.h"

#include "../schema/schema.h"
#include "../json_graphql/resolvers/resolverLoader.h"

void operation_convert(cJSON *definition, hashmap *resolvers, 
        ConfigEntry* configEntries, size_t numEntries, char *resolvers_filaname) {
    cJSON *operation_fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
    for (int j = 0; j < cJSON_GetArraySize(operation_fields); ++j)
    {
        int error;
        Operation *operation;
        cJSON *operation_field;
        cJSON *operation_field_name;
        cJSON *operation_field_name_value;
        char* operation_body;
        cJSON *operation_field_arguments;

        operation_field = cJSON_GetArrayItem(operation_fields, j);
        operation_field_name = cJSON_GetObjectItemCaseSensitive(operation_field, "name");
        operation_field_name_value = cJSON_GetObjectItemCaseSensitive(operation_field_name, "value");

        if (operation_field_name_value != NULL && (cJSON_IsString(operation_field_name_value) && (operation_field_name_value->valuestring != NULL))) {
            operation = (Operation*)malloc(sizeof(Operation));
            if (operation == NULL) {
                elog(LOG, "operation_convert(): operation struct malloc failed\n");
                goto parse_next_operation;
            }
            strcpy(operation->operationName, operation_field_name_value->valuestring);
            operation_field_arguments = cJSON_GetObjectItemCaseSensitive(operation_field, "arguments");
            operation->argumentsNumber = cJSON_GetArraySize(operation_field_arguments);
            for (size_t k = 0; k < operation->argumentsNumber; ++k) {
                Argument *currentArg;
                cJSON *argument;
                cJSON *argument_name;
                cJSON *argument_name_value;
                cJSON *argument_type;
                cJSON *argument_type_kind;
                cJSON *argument_type_name;
                cJSON *argument_type_name_value;
                cJSON *argument_type_type;
                cJSON *argument_type_type_name;
                cJSON *argument_type_type_name_value;
                cJSON *argument_defaultValue;
                cJSON *argument_defaultValue_value;
                char *converted_type;

                // create struct for new argument
                currentArg = (Argument*)malloc(sizeof(Argument));
                if (currentArg == NULL) {
                    elog(LOG, "operation_convert(): Argument malloc failed. Operation is invalid. Free operation\n");
                    free_arguments(operation);
                    free(operation);
                    goto parse_next_operation;
                }
                // parse argument name
                argument = cJSON_GetArrayItem(operation_field_arguments, k);
                argument_name = cJSON_GetObjectItemCaseSensitive(argument, "name");
                argument_name_value = cJSON_GetObjectItemCaseSensitive(argument_name, "value");
                if (argument_name_value != NULL && (cJSON_IsString(argument_name_value)) && (argument_name_value->valuestring != NULL)) {
                    strcpy(currentArg->argName, argument_name_value->valuestring);
                } else {
                    elog(LOG, "operation_convert(): Parse operation's name failed. Operation is invalid. Free operation\n");
                    free_arguments(operation);
                    free(operation);
                    goto parse_next_operation;
                }

                // parse argument kind
                argument_type = cJSON_GetObjectItemCaseSensitive(argument, "type");
                argument_type_kind = cJSON_GetObjectItemCaseSensitive(argument_type, "kind");
                if (argument_type_kind != NULL && (cJSON_IsString(argument_type_kind)) && (argument_type_kind->valuestring != NULL)) {
                    if (strcmp(argument_type_kind->valuestring, "NonNullType") == 0) {
                        currentArg->nonNullType = true;
                    } else {
                        currentArg->nonNullType = false;
                    }
                } else {
                    elog(LOG, "operation_convert(): Parse operation's kind failed. Operation is invalid. Free operation\n");
                    free_arguments(operation);
                    free(operation);
                    goto parse_next_operation;
                }

                // parse argument type
                argument_type_type = cJSON_GetObjectItemCaseSensitive(argument_type, "type");
                argument_type_name = cJSON_GetObjectItemCaseSensitive(argument_type, "name");
                if (argument_type_type != NULL) {
                    // nonNull type
                    argument_type_type_name = cJSON_GetObjectItemCaseSensitive(argument_type_type, "name");
                    argument_type_type_name_value = cJSON_GetObjectItemCaseSensitive(argument_type_type_name, "value");

                    if (argument_type_type_name_value != NULL && (cJSON_IsString(argument_type_type_name_value)) 
                            && (argument_type_type_name_value->valuestring != NULL)) {
                        converted_type = get_config_value(argument_type_type_name_value->valuestring, configEntries, numEntries);
                        strcpy(currentArg->argType, converted_type);
                    } else {
                        elog(LOG, "operation_convert(): Parse operation's type failed. Operation is invalid. Free operation\n");
                        free_arguments(operation);
                        free(operation);
                        goto parse_next_operation;
                    }
                } else if (argument_type_name != NULL) {
                    // nullable type
                    argument_type_name_value = cJSON_GetObjectItemCaseSensitive(argument_type_name, "value");

                    if (argument_type_name_value != NULL && (cJSON_IsString(argument_type_name_value)) 
                            && (argument_type_name_value->valuestring != NULL)) {
                        converted_type = get_config_value(argument_type_name_value->valuestring, configEntries, numEntries);
                        strcpy(currentArg->argType, converted_type);
                    } else {
                        elog(LOG, "operation_convert(): Parse operation's type failed. Operation is invalid. Free operation\n");
                        free_arguments(operation);
                        free(operation);
                        goto parse_next_operation;
                    }
                }

                // parse argument default value
                argument_defaultValue = cJSON_GetObjectItemCaseSensitive(argument, "defaultValue");
                argument_defaultValue_value = cJSON_GetObjectItemCaseSensitive(argument_defaultValue, "value");
                if (argument_defaultValue_value != NULL && (cJSON_IsString(argument_defaultValue_value)) 
                        && (argument_defaultValue_value->valuestring != NULL)) {
                    elog(LOG, "operation_convert(): default value is found\n");
                    strcpy(currentArg->defaultValue, argument_defaultValue_value->valuestring);
                } else {
                    elog(LOG, "operation_convert(): default value is not set\n");
                    currentArg->defaultValue[0] = '\0';
                }

                elog(LOG, "operation_convert(): argument[%ld] name: %s, type: %s, nonNull: %d, defaultValue: %s\n", k, currentArg->argName, currentArg->argType, currentArg->nonNullType, currentArg->defaultValue);
                // add argument into operation struct
                operation->arguments[k] = currentArg;
            }            

            // get sql-code for operation
            elog(LOG, "operation_convert(): load function body from %s\n", resolvers_filaname);
            operation_body = load_function_body(operation_field_name_value->valuestring, resolvers_filaname);
            if (operation_body != NULL) {
                elog(LOG, "operation_convert(): operation body for %s:\n\t\t%s\n", operation_field_name_value->valuestring, operation_body);
                // prepare stmt
                PGresult *stmt = prepare_statement(pg_conn, operation_field_name_value->valuestring, operation_body);
                // save to struct
                if (stmt == NULL) {
                    elog(LOG, "operation_convert(): Prepare statement failed\n");
                    free_arguments(operation);
                    free(operation);
                    goto parse_next_operation;
                } else {
                    operation->prepared_stmt = stmt;
                    elog(LOG, "operation_convert(): sql-query for operation %s is prepared\n", operation_field_name_value->valuestring);
                    error = hashmap_set(resolvers, strdup(operation_field_name_value->valuestring), strlen(operation_field_name_value->valuestring), (uintptr_t)operation);
                    if (error == -1) {
                        elog(LOG, "operation_convert(): hashmap_set error for operation %s: %p\n", operation_field_name_value->valuestring, operation_body);
                        free_arguments(operation);
                        free(operation);
                        goto parse_next_operation;
                    }
                }
            } else {
                elog(LOG, "operation_convert(): operation %s not found.\n", operation_field_name_value->valuestring);
                free_arguments(operation);
                free(operation);
                goto parse_next_operation;
            }
        }
parse_next_operation:
        elog(LOG, "operation_convert(): parse next operation\n");
    }
}