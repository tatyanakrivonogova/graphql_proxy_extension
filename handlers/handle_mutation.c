#include "handlers.h"
#include "../hashmap/map.h"
#include "../json_graphql/cJSON.h"
#include "../schema/schema_converting.h"

#include "postgres.h"

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

void insert_string(char *buffer, size_t buffer_size, const char *format, const char *value) {
    // find %
    const char *pos = strchr(format, '%');
    if (pos != NULL) {
        // copy string before %
        size_t prefix_length = pos - format;
        if (prefix_length + strlen(value) + strlen(pos + 1) < buffer_size) {
            strncpy(buffer, format, prefix_length);
            buffer[prefix_length] = '\0';

            strcat(buffer, value); // add string
            strcat(buffer, pos + 1); // add rest of string
        }
    } else {
        // copy original string if % is not in format
        elog(LOG, "copy original string\n");
        strncpy(buffer, format, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
    }
}

void handle_mutation(const char *json_query, hashmap *resolvers) {
    cJSON *json;
    cJSON *definitions;
    char *query;
    char *formatted_query;

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
                && (strcmp(definition_operation->valuestring, "mutation") != 0)) {
            elog(LOG, "Mutation operation expected\n");
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

            selection = cJSON_GetArrayItem(selections, j);
            selection_name = cJSON_GetObjectItemCaseSensitive(selection, "name");
            selection_name_value = cJSON_GetObjectItemCaseSensitive(selection_name, "value");
            if (selection_name_value != NULL && (cJSON_IsString(selection_name_value)) 
                    && (selection_name_value->valuestring != NULL))
                elog(LOG, "mutation %s is called\n", selection_name_value->valuestring);

            // get sql-code
            elog(LOG, "hashmap: %p\n", resolvers);

            uintptr_t res;
            if (hashmap_get(resolvers, selection_name_value->valuestring, strlen(selection_name_value->valuestring), &res)) {
                elog(LOG, "sql query for %s:\n\t\t%s\n", selection_name_value->valuestring, ((Mutation *)res)->mutationSql);
                // copy original query
                query = (char *)malloc(256);
                strcpy(query, ((Mutation *)res)->mutationSql);
                // query for setting argument's values
                formatted_query = (char *)malloc(256);
            } else {
                elog(LOG, "sql query for %s not found.\n", selection_name_value->valuestring);
                continue;
            }

            selection_arguments = cJSON_GetObjectItemCaseSensitive(selection, "arguments");
            for (int k = 0; k < cJSON_GetArraySize(selection_arguments); ++k)
            {
                cJSON *argument;
                cJSON *argument_name;
                cJSON *argument_name_value;
                cJSON *argument_value;
                cJSON *arguement_value_kind;
                cJSON *argument_value_value;

                argument = cJSON_GetArrayItem(selection_arguments, k);
                argument_name = cJSON_GetObjectItemCaseSensitive(argument, "name");
                argument_name_value = cJSON_GetObjectItemCaseSensitive(argument_name, "value");
                if (argument_name_value != NULL && (cJSON_IsString(argument_name_value)) 
                    && (argument_name_value->valuestring != NULL))
                    elog(LOG, "argument[%d] name: %s\n", k, argument_name_value->valuestring);

                argument_value = cJSON_GetObjectItemCaseSensitive(argument, "value");
                arguement_value_kind = cJSON_GetObjectItemCaseSensitive(argument_value, "kind");
                if (arguement_value_kind != NULL && (cJSON_IsString(arguement_value_kind)) 
                    && (arguement_value_kind->valuestring != NULL))
                    elog(LOG, "argument[%d] kind: %s\n", k, arguement_value_kind->valuestring);
                argument_value_value = cJSON_GetObjectItemCaseSensitive(argument_value, "value");
                if (argument_value_value != NULL && (cJSON_IsString(argument_value_value)) 
                    && (argument_value_value->valuestring != NULL)) {
                    elog(LOG, "string argument[%d] value: %s\n", k, argument_value_value->valuestring);
                    // set arguments into mutation function
                    elog(LOG, "before sprintf formatted_query: %s query: %s value: %s\n", formatted_query, query, argument_value_value->valuestring);
                    insert_string(formatted_query, 256, query, argument_value_value->valuestring);
                    elog(LOG, "after sprintf\n");
                }

                if (argument_value_value != NULL && (cJSON_IsNumber(argument_value_value))) {
                    elog(LOG, "int argument[%d] value: %d\n", k, argument_value_value->valueint);
                    // set arguments into mutation function
                    insert_string(formatted_query, 256, query, argument_value_value->valueint);
                }
                swap(&query, &formatted_query);
            }

            elog(LOG, "Formatted query: %s\n", formatted_query);
            // execute formatted query
            elog(LOG, "before free\n");
            free(query);
            free(formatted_query);
        }
    }
}