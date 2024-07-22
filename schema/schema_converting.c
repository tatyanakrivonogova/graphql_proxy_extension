#include "schema_converting.h"

#include <stdbool.h>

#include "postgres.h"
#include "../json_graphql/cJSON.h"
#include "../json_graphql/config/config.h"
#include "../json_graphql/resolvers/resolverLoader.h"


bool isTypeExists(char* type_name) {
    for (size_t i = 0; i < types.numCreatedTypes; ++i) {
        if (strcmp(type_name, types.createdTypes[i]) == 0) return true;
    }
    return false;
}


void createForeignKey(char* sql_alter, char* table_name, char* another_table_name, char* field_name) {
    // query for adding foreign key
    strcat(sql_alter, "ALTER TABLE ");
    strcat(sql_alter, table_name);
    strcat(sql_alter, " ADD CONSTRAINT fk_");
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

void freeAlterQueries(char** sql_alter_queries, size_t sql_alter_queries_num) {
    for (size_t i = 0; i < sql_alter_queries_num; ++i) {
        free(sql_alter_queries[i]);
    }
}

void schema_convert(char *json_schema) { 
    char *sql_create;
    char *sql_alter_queries[ALTER_QUERIES_NUMBER];
    size_t sql_alter_queries_num;

    const char* filename = "../contrib/graphql_proxy/schema/config.txt";
    size_t numEntries;
    ConfigEntry* configEntries = loadConfigFile(filename, &numEntries);
    if (configEntries == NULL) return;

	// open the file
	// FILE *fp = fopen("data.json", "r");
	// if (fp == NULL) {
	// 	elog(LOG, "Error: Unable to open the file.\n");
    //     freeConfig(configEntries, numEntries);
	// 	return;
	// }

	// read the file contents into a string
	// char json_schema[10000];
	// int len = fread(json_schema, 1, sizeof(json_schema), fp);
	// fclose(fp);

	// parse the JSON data
	cJSON *json = cJSON_Parse(json_schema);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "Error: %s %ld\n", error_ptr, error_ptr - json_schema);
		}
		cJSON_Delete(json);
        freeConfig(configEntries, numEntries);
		return;
	}

    sql_create = (char*)calloc(QUERY_LENGTH, sizeof(char));
    sql_alter_queries_num = 0;

	// access the JSON data
	cJSON *definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    elog(LOG, "definitions: %p\n", definitions);

    types.numCreatedTypes = 0;
    queries.numCreatedQueries = 0;
    mutations.numCreatedMutations = 0;
    for (int i = 0; i < cJSON_GetArraySize(definitions); ++i)
    {
        cJSON *definition = cJSON_GetArrayItem(definitions, i);
        cJSON *definition_kind = cJSON_GetObjectItemCaseSensitive(definition, "kind");
        if (definition_kind != NULL && (cJSON_IsString(definition_kind)) && (definition_kind->valuestring != NULL)
                && (strcmp(definition_kind->valuestring, "ObjectTypeDefinition") != 0)) continue;

        cJSON *definition_name = cJSON_GetObjectItemCaseSensitive(definition, "name");
        cJSON *definition_name_value = cJSON_GetObjectItemCaseSensitive(definition_name, "value");
        if (definition_name_value != NULL && (cJSON_IsString(definition_name_value)) && (definition_name_value->valuestring != NULL)) {
            if (strcmp(definition_name_value->valuestring, "Query") == 0) {
                // parse query name
                cJSON *query_fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
                for (int j = 0; j < cJSON_GetArraySize(query_fields); ++j)
                {
                    cJSON *query_field = cJSON_GetArrayItem(query_fields, j);
                    cJSON *query_field_name = cJSON_GetObjectItemCaseSensitive(query_field, "name");
                    cJSON *query_field_name_value = cJSON_GetObjectItemCaseSensitive(query_field_name, "value");
                    // elog(LOG, "Query %s\n", query_field_name_value->valuestring);
                    if (query_field_name_value != NULL && (cJSON_IsString(query_field_name_value) && (query_field_name_value->valuestring != NULL))) {
                        strcpy(queries.createdQueries[queries.numCreatedQueries++], query_field_name_value->valuestring);
                    }

                    // get sql-code for query
                    char* query_body = load_function_body(query_field_name_value->valuestring);
                    if (query_body != NULL) {
                        char *formatted_query = (char*)malloc(QUERY_LENGTH);
                        sprintf(formatted_query, query_body, 10);
                        elog(LOG, "Query body for %s:\n\t\t%s\n", query_field_name_value->valuestring, formatted_query);
                        free(query_body);
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
                    cJSON *mutation_field = cJSON_GetArrayItem(mutation_fields, j);
                    cJSON *mutation_field_name = cJSON_GetObjectItemCaseSensitive(mutation_field, "name");
                    cJSON *mutation_field_name_value = cJSON_GetObjectItemCaseSensitive(mutation_field_name, "value");
                    // elog(LOG, "Mutation %s\n", mutation_field_name_value->valuestring);
                    if (mutation_field_name_value != NULL && (cJSON_IsString(mutation_field_name_value) && (mutation_field_name_value->valuestring != NULL))) {
                        strcpy(mutations.createdMutations[mutations.numCreatedMutations++], mutation_field_name_value->valuestring);
                    }

                    // get sql-code for mutation
                    char* mutation_body = load_function_body(mutation_field_name_value->valuestring);
                    if (mutation_body != NULL) {
                        elog(LOG, "Mutation body for %s:\n\t\t%s\n", mutation_field_name_value->valuestring, mutation_body);
                        free(mutation_body);
                    } else {
                        elog(LOG, "Mutation %s not found.\n", mutation_field_name_value->valuestring);
                    }
                }
                continue;
            }
        }

        
        strcpy(sql_create, "CREATE TABLE ");
        cJSON *type_name = cJSON_GetObjectItemCaseSensitive(definition, "name");
        cJSON *type_name_value = cJSON_GetObjectItemCaseSensitive(type_name, "value");

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

        cJSON *fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
        for (int j = 0; j < cJSON_GetArraySize(fields); ++j)
        {
            strcat(sql_create, ", ");
            cJSON *field = cJSON_GetArrayItem(fields, j);
            cJSON *kind_field = cJSON_GetObjectItemCaseSensitive(field, "kind");
            if (cJSON_IsString(kind_field) && (kind_field->valuestring != NULL) 
                && (strcmp(kind_field->valuestring, "FieldDefinition") != 0)) continue;
            cJSON *field_name = cJSON_GetObjectItemCaseSensitive(field, "name");
            cJSON *field_name_value = cJSON_GetObjectItemCaseSensitive(field_name, "value");

            // found name of field
            if (cJSON_IsString(field_name_value) && (field_name_value->valuestring != NULL)) {
                elog(LOG, "\tfield[%d]: %s\n", j, field_name_value->valuestring);
                strcat(sql_create, field_name_value->valuestring);
                strcat(sql_create, " ");
            }

            cJSON *field_type = cJSON_GetObjectItemCaseSensitive(field, "type");
            cJSON *field_type_type = cJSON_GetObjectItemCaseSensitive(field_type, "type");
            cJSON *field_type_type_kind = cJSON_GetObjectItemCaseSensitive(field_type_type, "kind");
            if (field_type_type_kind != NULL && (cJSON_IsString(field_type_type_kind)) 
                && (field_type_type_kind->valuestring != NULL) && (strcmp(field_type_type_kind->valuestring, "NamedType") == 0))
            {
                // NamedType
                cJSON *field_type_type_name = cJSON_GetObjectItemCaseSensitive(field_type_type, "name");
                cJSON *field_type_type_name_value = cJSON_GetObjectItemCaseSensitive(field_type_type_name, "value");

                // found type of field
                if (cJSON_IsString(field_type_type_name_value) && (field_type_type_name_value->valuestring != NULL)) {
                    elog(LOG, "\t\tfield_type[%d]: %s\n", j, field_type_type_name_value->valuestring);
                    char *convertedType = getConfigValue(field_type_type_name_value->valuestring, configEntries, numEntries);
                    if (convertedType == NULL) {
                        if (isTypeExists(field_type_type_name_value->valuestring)) {
                            strcat(sql_create, "INT ");

                            sql_alter_queries[sql_alter_queries_num] = (char*)calloc(QUERY_LENGTH, sizeof(char));
                            createForeignKey(sql_alter_queries[sql_alter_queries_num], 
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
                cJSON *field_type_type_type = cJSON_GetObjectItemCaseSensitive(field_type_type, "type");
                cJSON *field_type_type_type_type = cJSON_GetObjectItemCaseSensitive(field_type_type_type, "type");
                cJSON *field_type_type_type_type_kind = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type, "kind");
                if (field_type_type_type_type_kind != NULL && (cJSON_IsString(field_type_type_type_type_kind)) 
                    && (strcmp(field_type_type_type_type_kind->valuestring, "NamedType") == 0))
                {
                    // List of NamedType
                    cJSON *field_type_type_type_type_name = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type, "name");
                    cJSON *field_type_type_type_type_name_value = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type_name, "value");

                    // found type of field
                    if (field_type_type_type_type_name_value != NULL && (cJSON_IsString(field_type_type_type_type_name_value)) 
                        && (field_type_type_type_type_name_value->valuestring != NULL)) {
                        elog(LOG, "\t\tfield_type[%d]: List of %s\n", j, field_type_type_type_type_name_value->valuestring);
                        char *convertedType = getConfigValue(field_type_type_type_type_name_value->valuestring, configEntries, numEntries);
                        if (convertedType == NULL) {
                            elog(LOG, "Type is not found: %s\n", field_type_type_type_type_name_value->valuestring);
                        } else {
                            strcat(sql_create, convertedType);
                            strcat(sql_create, " ARRAY ");
                        }
                    }
                } else {
                    elog(LOG, "Nested lists is not supported\n");
                    return;
                }
            }
            
            cJSON *field_type_kind = cJSON_GetObjectItemCaseSensitive(field_type, "kind");

            // found kind of field
            if (cJSON_IsString(field_type_kind) && (field_type_kind->valuestring != NULL)) {
                elog(LOG, "\t\tfield_kind[%d]: %s\n", j, field_type_kind->valuestring);
                if (strcmp(field_type_kind->valuestring, "NonNullType") == 0)
                    strcat(sql_create, "NOT NULL");
            }
        }
        strcat(sql_create, ");");
        elog(LOG, "sql_create query: %s\n", sql_create);

        memset(sql_create, 0, QUERY_LENGTH);
        freeAlterQueries(sql_alter_queries, sql_alter_queries_num);
    }

    free(sql_create);
    freeConfig(configEntries, numEntries);
	// delete the JSON object
	cJSON_Delete(json);
}
