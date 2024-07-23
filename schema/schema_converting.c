#include "schema_converting.h"

#include <stdbool.h>

#include "postgres.h"
#include "../json_graphql/cJSON.h"
#include "../json_graphql/config/config.h"
#include "../json_graphql/resolvers/resolverLoader.h"
#include "../libpq/query_executing.c"
#include "../hashmap/map.h"


bool is_type_exists(char* type_name) {
    for (size_t i = 0; i < types.numCreatedTypes; ++i) {
        if (strcmp(type_name, types.createdTypes[i]) == 0) return true;
    }
    return false;
}


void create_foreign_key(char* sql_alter, char* table_name, char* another_table_name, char* field_name) {
    // query for adding foreign key
    strcat(sql_alter, "ALTER TABLE ");
    strcat(sql_alter, table_name);
    strcat(sql_alter, " ADD CONSTRAINT IF NOT EXISTS fk_");
    strcat(sql_alter, table_name);
    strcat(sql_alter, "_");
    strcat(sql_alter, another_table_name);
    strcat(sql_alter, "_");
    strcat(sql_alter, field_name);
    strcat(sql_alter, " FOREIGN KEY (");
    strcat(sql_alter, field_name);
    strcat(sql_alter, ") REFERENCES ");
    strcat(sql_alter, another_table_name);
    strcat(sql_alter, "(pk_");
    strcat(sql_alter, another_table_name);
    strcat(sql_alter, ");");
}

void free_alter_queries(char** sql_alter_queries, size_t sql_alter_queries_num) {
    for (size_t i = 0; i < sql_alter_queries_num; ++i) {
        free(sql_alter_queries[i]);
    }
}

hashmap *schema_convert(const char *json_schema) {
    char *sql_create;
    char *sql_alter_queries[ALTER_QUERIES_NUMBER];
    size_t sql_alter_queries_num;
    const char* filename;
    size_t numEntries;
    ConfigEntry* configEntries;
    cJSON *json;
    cJSON *definitions;
    char *conn_info = "dbname=postgres host=localhost port=5432";
    PGconn *conn;
    PGresult *res;

    hashmap *resolvers = hashmap_create();
	if (resolvers == NULL) {
		printf("schema_convert(): failed while hashmap init\n");
		return NULL;
	}

    if (!create_connection(&conn, conn_info)) {
        return NULL;
    }

    filename = "../contrib/graphql_proxy/schema/config.txt";
    configEntries = load_config_file(filename, &numEntries);
    if (configEntries == NULL) return NULL;

	// parse the JSON data
	json = cJSON_Parse(json_schema);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "Error: %s %ld\n", error_ptr, error_ptr - json_schema);
		}
		cJSON_Delete(json);
        free_config(configEntries, numEntries);
		return NULL;
	}

    sql_create = (char*)calloc(QUERY_LENGTH, sizeof(char));
    sql_alter_queries_num = 0;

	// access the JSON data
	definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    elog(LOG, "definitions: %p\n", definitions);

    types.numCreatedTypes = 0;
    queries.numCreatedQueries = 0;
    mutations.numCreatedMutations = 0;
    for (int i = 0; i < cJSON_GetArraySize(definitions); ++i)
    {
        cJSON *definition;
        cJSON *definition_kind;
        cJSON *definition_name;
        cJSON *definition_name_value;
        cJSON *type_name;
        cJSON *type_name_value;
        cJSON *fields;
        
        definition = cJSON_GetArrayItem(definitions, i);
        definition_kind = cJSON_GetObjectItemCaseSensitive(definition, "kind");
        if (definition_kind != NULL && (cJSON_IsString(definition_kind)) && (definition_kind->valuestring != NULL)
                && (strcmp(definition_kind->valuestring, "ObjectTypeDefinition") != 0)) continue;

        definition_name = cJSON_GetObjectItemCaseSensitive(definition, "name");
        definition_name_value = cJSON_GetObjectItemCaseSensitive(definition_name, "value");
        if (definition_name_value != NULL && (cJSON_IsString(definition_name_value)) && (definition_name_value->valuestring != NULL)) {
            if (strcmp(definition_name_value->valuestring, "Query") == 0) {
                // parse query name
                cJSON *query_fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
                for (int j = 0; j < cJSON_GetArraySize(query_fields); ++j)
                {
                    int error;
                    cJSON *query_field;
                    cJSON *query_field_name;
                    cJSON *query_field_name_value;
                    char* query_body;

                    query_field = cJSON_GetArrayItem(query_fields, j);
                    query_field_name = cJSON_GetObjectItemCaseSensitive(query_field, "name");
                    query_field_name_value = cJSON_GetObjectItemCaseSensitive(query_field_name, "value");
                    // elog(LOG, "Query %s\n", query_field_name_value->valuestring);
                    if (query_field_name_value != NULL && (cJSON_IsString(query_field_name_value) && (query_field_name_value->valuestring != NULL))) {
                        strcpy(queries.createdQueries[queries.numCreatedQueries++], query_field_name_value->valuestring);
                    }

                    // get sql-code for query
                    query_body = load_function_body(query_field_name_value->valuestring);
                    if (query_body != NULL) {
                        char *formatted_query;

                        error = hashmap_set(resolvers, strdup(query_field_name_value->valuestring), strlen(query_field_name_value->valuestring), (uintptr_t)query_body);
                        if (error == -1)
                            elog(LOG, "hashmap_set error for query %s: %p\n", query_field_name_value->valuestring, query_body);

                        formatted_query = (char*)malloc(QUERY_LENGTH);
                        sprintf(formatted_query, query_body, 10);
                        elog(LOG, "Query body for %s:\n\t\t%s\n", query_field_name_value->valuestring, formatted_query);
                        // free(query_body);
                        free(formatted_query);
                    } else {
                        elog(LOG, "Query %s not found.\n", query_field_name_value->valuestring);
                    }
                }
                continue;
            } else if (strcmp(definition_name_value->valuestring, "Mutation") == 0) {
                // load sql-code for this mutation
                cJSON *mutation_fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
                for (int j = 0; j < cJSON_GetArraySize(mutation_fields); ++j)
                {
                    int error;
                    cJSON *mutation_field;
                    cJSON *mutation_field_name;
                    cJSON *mutation_field_name_value;
                    char* mutation_body;
                    cJSON *mutation_field_arguments;

                    mutation_field = cJSON_GetArrayItem(mutation_fields, j);
                    mutation_field_name = cJSON_GetObjectItemCaseSensitive(mutation_field, "name");
                    mutation_field_name_value = cJSON_GetObjectItemCaseSensitive(mutation_field_name, "value");
                    // elog(LOG, "Mutation %s\n", mutation_field_name_value->valuestring);
                    if (mutation_field_name_value != NULL && (cJSON_IsString(mutation_field_name_value) && (mutation_field_name_value->valuestring != NULL))) {
                        // strcpy(mutations.createdMutations[mutations.numCreatedMutations++], mutation_field_name_value->valuestring);
                        mutations.createdMutations[mutations.numCreatedMutations] = (Mutation*)malloc(sizeof(Mutation));
                        if (mutations.createdMutations[mutations.numCreatedMutations] == NULL) {
                            elog(LOG, "Mutation struct malloc failed\n");
                            continue;
                        }
                        strcpy(mutations.createdMutations[mutations.numCreatedMutations]->mutationName, mutation_field_name_value->valuestring);
                        mutation_field_arguments = cJSON_GetObjectItemCaseSensitive(mutation_field, "arguments");
                        mutations.createdMutations[mutations.numCreatedMutations]->argumentsNumber = cJSON_GetArraySize(mutation_field_arguments);
                        for (size_t k = 0; k < mutations.createdMutations[mutations.numCreatedMutations]->argumentsNumber; ++k) {
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
                                elog(LOG, "Argument malloc failed\n");
                            }
                            // parse argument name
                            argument = cJSON_GetArrayItem(mutation_field_arguments, k);
                            argument_name = cJSON_GetObjectItemCaseSensitive(argument, "name");
                            argument_name_value = cJSON_GetObjectItemCaseSensitive(argument_name, "value");
                            if (argument_name_value != NULL && (cJSON_IsString(argument_name_value)) && (argument_name_value->valuestring != NULL)) {
                                strcpy(currentArg->argName, argument_name_value->valuestring);
                            } else {
                                free(currentArg);
                                continue;
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
                                free(currentArg);
                                continue;
                            }

                            // parse argument type
                            argument_type_type = cJSON_GetObjectItemCaseSensitive(argument_type, "type");
                            argument_type_type_name = cJSON_GetObjectItemCaseSensitive(argument_type_type, "name");
                            argument_type_type_name_value = cJSON_GetObjectItemCaseSensitive(argument_type_type_name, "value");

                            if (argument_type_type_name_value != NULL && (cJSON_IsString(argument_type_type_name_value)) && (argument_type_type_name_value->valuestring != NULL)) {
                                strcpy(currentArg->argType, argument_type_type_name_value->valuestring);
                            } else {
                                free(currentArg);
                                continue;
                            }

                            elog(LOG, "argument[%ld] name: %s, type: %s, nonNull: %d\n", k, currentArg->argName, currentArg->argType, currentArg->nonNullType);
                            // add argument into mutation struct
                            mutations.createdMutations[mutations.numCreatedMutations]->arguments[k] = currentArg;
                        }
                        mutations.numCreatedMutations++;
                    }

                    // get sql-code for mutation
                    mutation_body = load_function_body(mutation_field_name_value->valuestring);
                    error = hashmap_set(resolvers, strdup(mutation_field_name_value->valuestring), strlen(mutation_field_name_value->valuestring), (uintptr_t)mutation_body);
                        if (error == -1)
                            elog(LOG, "hashmap_set error for mutation %s: %p\n", mutation_field_name_value->valuestring, mutation_body);

                    if (mutation_body != NULL) {
                        elog(LOG, "Mutation body for %s:\n\t\t%s\n", mutation_field_name_value->valuestring, mutation_body);
                        // free(mutation_body);
                    } else {
                        elog(LOG, "Mutation %s not found.\n", mutation_field_name_value->valuestring);
                    }
                }
                continue;
            }
        }

        
        strcpy(sql_create, "CREATE TABLE IF NOT EXISTS ");
        type_name = cJSON_GetObjectItemCaseSensitive(definition, "name");
        type_name_value = cJSON_GetObjectItemCaseSensitive(type_name, "value");

        // found name of table
        if (cJSON_IsString(type_name_value) && (type_name_value->valuestring != NULL)) {
            elog(LOG, "table[%d]: %s\n", i, type_name_value->valuestring);
            strcat(sql_create, type_name_value->valuestring);
            strcat(sql_create, "(");

            // add surrogate primary key to table
            strcat(sql_create, "pk_");
            strcat(sql_create, type_name_value->valuestring);
            strcat(sql_create, " SERIAL PRIMARY KEY");

            // add type to types array
            strcpy(types.createdTypes[types.numCreatedTypes++], type_name_value->valuestring);
            elog(LOG, "--- types number: %ld\n", types.numCreatedTypes);
        }

        fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
        for (int j = 0; j < cJSON_GetArraySize(fields); ++j)
        {
            cJSON *field;
            cJSON *kind_field;
            cJSON *field_name;
            cJSON *field_name_value;
            cJSON *field_type;
            cJSON *field_type_type;
            cJSON *field_type_type_kind;
            cJSON *field_type_kind;

            strcat(sql_create, ", ");
            field = cJSON_GetArrayItem(fields, j);
            kind_field = cJSON_GetObjectItemCaseSensitive(field, "kind");
            if (cJSON_IsString(kind_field) && (kind_field->valuestring != NULL) 
                && (strcmp(kind_field->valuestring, "FieldDefinition") != 0)) continue;
            field_name = cJSON_GetObjectItemCaseSensitive(field, "name");
            field_name_value = cJSON_GetObjectItemCaseSensitive(field_name, "value");

            // found name of field
            if (cJSON_IsString(field_name_value) && (field_name_value->valuestring != NULL)) {
                elog(LOG, "\tfield[%d]: %s\n", j, field_name_value->valuestring);
                strcat(sql_create, field_name_value->valuestring);
                strcat(sql_create, " ");
            }

            field_type = cJSON_GetObjectItemCaseSensitive(field, "type");
            field_type_type = cJSON_GetObjectItemCaseSensitive(field_type, "type");
            field_type_type_kind = cJSON_GetObjectItemCaseSensitive(field_type_type, "kind");
            if (field_type_type_kind != NULL && (cJSON_IsString(field_type_type_kind)) 
                && (field_type_type_kind->valuestring != NULL) && (strcmp(field_type_type_kind->valuestring, "NamedType") == 0))
            {
                // NamedType
                cJSON *field_type_type_name;
                cJSON *field_type_type_name_value;

                field_type_type_name = cJSON_GetObjectItemCaseSensitive(field_type_type, "name");
                field_type_type_name_value = cJSON_GetObjectItemCaseSensitive(field_type_type_name, "value");

                // found type of field
                if (cJSON_IsString(field_type_type_name_value) && (field_type_type_name_value->valuestring != NULL)) {
                    char *convertedType;
                    
                    elog(LOG, "\t\tfield_type[%d]: %s\n", j, field_type_type_name_value->valuestring);
                    convertedType = get_config_value(field_type_type_name_value->valuestring, configEntries, numEntries);
                    if (convertedType == NULL) {
                        if (is_type_exists(field_type_type_name_value->valuestring)) {
                            strcat(sql_create, "INT ");

                            sql_alter_queries[sql_alter_queries_num] = (char*)calloc(QUERY_LENGTH, sizeof(char));
                            create_foreign_key(sql_alter_queries[sql_alter_queries_num], 
                                             type_name_value->valuestring, 
                                             field_type_type_name_value->valuestring, 
                                             field_name_value->valuestring);

                            elog(LOG, "sql_alter: %s\n", sql_alter_queries[sql_alter_queries_num]);
                            ++sql_alter_queries_num;
                        } else {
                            elog(LOG, "Type is not found: %s\n", field_type_type_name_value->valuestring);
                        }
                    } else {
                        strcat(sql_create, convertedType);
                        strcat(sql_create, " ");
                    }
                }
            }
            else if (field_type_type_kind != NULL && (cJSON_IsString(field_type_type_kind)) 
                && (field_type_type_kind->valuestring != NULL) && (strcmp(field_type_type_kind->valuestring, "ListType") == 0))
            {
                // ListType
                cJSON *field_type_type_type;
                cJSON *field_type_type_type_type;
                cJSON *field_type_type_type_type_kind;

                field_type_type_type = cJSON_GetObjectItemCaseSensitive(field_type_type, "type");
                field_type_type_type_type = cJSON_GetObjectItemCaseSensitive(field_type_type_type, "type");
                field_type_type_type_type_kind = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type, "kind");
                if (field_type_type_type_type_kind != NULL && (cJSON_IsString(field_type_type_type_type_kind)) 
                    && (strcmp(field_type_type_type_type_kind->valuestring, "NamedType") == 0))
                {
                    // List of NamedType
                    cJSON *field_type_type_type_type_name;
                    cJSON *field_type_type_type_type_name_value;

                    field_type_type_type_type_name = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type, "name");
                    field_type_type_type_type_name_value = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type_name, "value");

                    // found type of field
                    if (field_type_type_type_type_name_value != NULL && (cJSON_IsString(field_type_type_type_type_name_value)) 
                        && (field_type_type_type_type_name_value->valuestring != NULL)) {
                        char *convertedType;
                        
                        elog(LOG, "\t\tfield_type[%d]: List of %s\n", j, field_type_type_type_type_name_value->valuestring);
                        convertedType = get_config_value(field_type_type_type_type_name_value->valuestring, configEntries, numEntries);
                        if (convertedType == NULL) {
                            elog(LOG, "Type is not found: %s\n", field_type_type_type_type_name_value->valuestring);
                        } else {
                            strcat(sql_create, convertedType);
                            strcat(sql_create, " ARRAY ");
                        }
                    }
                } else {
                    elog(LOG, "Nested lists is not supported\n");
                    return NULL;
                }
            }
            
            field_type_kind = cJSON_GetObjectItemCaseSensitive(field_type, "kind");

            // found kind of field
            if (cJSON_IsString(field_type_kind) && (field_type_kind->valuestring != NULL)) {
                elog(LOG, "\t\tfield_kind[%d]: %s\n", j, field_type_kind->valuestring);
                if (strcmp(field_type_kind->valuestring, "NonNullType") == 0)
                    strcat(sql_create, "NOT NULL");
            }
        }
        strcat(sql_create, ");");
        elog(LOG, "sql_create query: %s\n", sql_create);

        // execute create table query
        exec_query(&conn, sql_create, &res);
        handle_query(res);
        

        for (size_t i = 0; i < sql_alter_queries_num; ++i) {
            exec_query(&conn, sql_create, &res);
            handle_query(res);
        }

        memset(sql_create, 0, QUERY_LENGTH);
        free_alter_queries(sql_alter_queries, sql_alter_queries_num);
    }

    free(sql_create);
    free_config(configEntries, numEntries);
	// delete the JSON object
	cJSON_Delete(json);

    //clear used resources
    PQclear(res);
    close_connection(&conn);

    return resolvers;
}
