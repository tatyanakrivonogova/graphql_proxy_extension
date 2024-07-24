#include "mutation_converting.h"

#include "postgres.h"

#include "../schema/schema.h"
#include "../json_graphql/resolvers/resolverLoader.h"

void mutation_convert(cJSON *definition, hashmap *resolvers) {
    cJSON *mutation_fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
    for (int j = 0; j < cJSON_GetArraySize(mutation_fields); ++j)
    {
        int error;
        Mutation *mutation;
        cJSON *mutation_field;
        cJSON *mutation_field_name;
        cJSON *mutation_field_name_value;
        char* mutation_body;
        cJSON *mutation_field_arguments;

        mutation_field = cJSON_GetArrayItem(mutation_fields, j);
        mutation_field_name = cJSON_GetObjectItemCaseSensitive(mutation_field, "name");
        mutation_field_name_value = cJSON_GetObjectItemCaseSensitive(mutation_field_name, "value");

        if (mutation_field_name_value != NULL && (cJSON_IsString(mutation_field_name_value) && (mutation_field_name_value->valuestring != NULL))) {
            mutation = (Mutation*)malloc(sizeof(Mutation));
            if (mutation == NULL) {
                elog(LOG, "Mutation struct malloc failed\n");
                goto parse_next_mutation;
            }
            strcpy(mutation->mutationName, mutation_field_name_value->valuestring);
            mutation_field_arguments = cJSON_GetObjectItemCaseSensitive(mutation_field, "arguments");
            mutation->argumentsNumber = cJSON_GetArraySize(mutation_field_arguments);
            for (size_t k = 0; k < mutation->argumentsNumber; ++k) {
                Argument *currentArg;
                cJSON *argument;
                cJSON *argument_name;
                cJSON *argument_name_value;
                cJSON *argument_type;
                cJSON *argument_type_kind;
                cJSON *argument_type_type;
                cJSON *argument_type_type_name;
                cJSON *argument_type_type_name_value;

                // create struct for new argument
                currentArg = (Argument*)malloc(sizeof(Argument));
                if (currentArg == NULL) {
                    elog(LOG, "Argument malloc failed. Mutation is invalid. Free mutation\n");
                    free_arguments(mutation);
                    free(mutation);
                    goto parse_next_mutation;
                }
                // parse argument name
                argument = cJSON_GetArrayItem(mutation_field_arguments, k);
                argument_name = cJSON_GetObjectItemCaseSensitive(argument, "name");
                argument_name_value = cJSON_GetObjectItemCaseSensitive(argument_name, "value");
                if (argument_name_value != NULL && (cJSON_IsString(argument_name_value)) && (argument_name_value->valuestring != NULL)) {
                    strcpy(currentArg->argName, argument_name_value->valuestring);
                } else {
                    elog(LOG, "Parse mutation's name failed. Mutation is invalid. Free mutation\n");
                    free_arguments(mutation);
                    free(mutation);
                    goto parse_next_mutation;
                }

                // parse argumentkind
                argument_type = cJSON_GetObjectItemCaseSensitive(argument, "type");
                argument_type_kind = cJSON_GetObjectItemCaseSensitive(argument_type, "kind");
                if (argument_type_kind != NULL && (cJSON_IsString(argument_type_kind)) && (argument_type_kind->valuestring != NULL)) {
                    if (strcmp(argument_type_kind->valuestring, "NonNullType") == 0) {
                        currentArg->nonNullType = true;
                    } else {
                        currentArg->nonNullType = false;
                    }
                } else {
                    elog(LOG, "Parse mutation's kind failed. Mutation is invalid. Free mutation\n");
                    free_arguments(mutation);
                    free(mutation);
                    goto parse_next_mutation;
                }

                // parse argument type
                argument_type_type = cJSON_GetObjectItemCaseSensitive(argument_type, "type");
                argument_type_type_name = cJSON_GetObjectItemCaseSensitive(argument_type_type, "name");
                argument_type_type_name_value = cJSON_GetObjectItemCaseSensitive(argument_type_type_name, "value");

                if (argument_type_type_name_value != NULL && (cJSON_IsString(argument_type_type_name_value)) && (argument_type_type_name_value->valuestring != NULL)) {
                    strcpy(currentArg->argType, argument_type_type_name_value->valuestring);
                } else {
                    elog(LOG, "Parse mutation's type failed. Mutation is invalid. Free mutation\n");
                    free_arguments(mutation);
                    free(mutation);
                    goto parse_next_mutation;
                }

                elog(LOG, "argument[%ld] name: %s, type: %s, nonNull: %d\n", k, currentArg->argName, currentArg->argType, currentArg->nonNullType);
                // add argument into mutation struct
                mutation->arguments[k] = currentArg;
            }            

            // get sql-code for mutation
            mutation_body = load_function_body(mutation_field_name_value->valuestring);
            if (mutation_body != NULL) {
                elog(LOG, "Mutation body for %s:\n\t\t%s\n", mutation_field_name_value->valuestring, mutation_body);
                mutation->mutationSql = mutation_body;
                error = hashmap_set(resolvers, strdup(mutation_field_name_value->valuestring), strlen(mutation_field_name_value->valuestring), (uintptr_t)mutation);
                if (error == -1)
                    elog(LOG, "hashmap_set error for mutation %s: %p\n", mutation_field_name_value->valuestring, mutation_body);
            } else {
                elog(LOG, "Mutation %s not found.\n", mutation_field_name_value->valuestring);
                free_arguments(mutation);
                free(mutation);
                goto parse_next_mutation;
            }
        }
parse_next_mutation:
        elog(LOG, "parse next mutation\n");
    }
    
}