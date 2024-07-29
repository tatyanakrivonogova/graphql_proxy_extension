#include "schema_converting.h"

#include "postgres.h"

#include "../json_graphql/cJSON.h"
#include "../json_graphql/resolvers/resolverLoader.h"
#include "../postgres_connect/postgres_connect.h"
#include "../hashmap/map.h"
#include "operation_converting.h"
#include "../defines.h"


bool is_type_exists(char* type_name) {
    for (size_t i = 0; i < types.numCreatedTypes; ++i) {
        if (strcmp(type_name, types.createdTypes[i]) == 0) return true;
    }
    return false;
}

void free_sql_alter_queries(char **sql_alter_queries, size_t num) {
    for (size_t i = 0; i < num; ++i)
        if (sql_alter_queries[i] != NULL) free(sql_alter_queries[i]);
}


void create_foreign_key(char* sql_alter, char* table_name, char* another_table_name, char* field_name) {
    // drop old constraint
    // "ALTER TABLE Message DROP CONSTRAINT IF EXISTS fk_Message_Person_sender;";
    strcat(sql_alter, "ALTER TABLE graphql_proxy.");
    strcat(sql_alter, table_name);
    strcat(sql_alter, " DROP CONSTRAINT IF EXISTS fk_");
    strcat(sql_alter, table_name);
    strcat(sql_alter, "_");
    strcat(sql_alter, another_table_name);
    strcat(sql_alter, "_");
    strcat(sql_alter, field_name);
    strcat(sql_alter, ";");
    
    // query for adding foreign key   
    strcat(sql_alter, "ALTER TABLE graphql_proxy.");
    strcat(sql_alter, table_name);
    strcat(sql_alter, " ADD CONSTRAINT fk_");
    strcat(sql_alter, table_name);
    strcat(sql_alter, "_");
    strcat(sql_alter, another_table_name);
    strcat(sql_alter, "_");
    strcat(sql_alter, field_name);
    strcat(sql_alter, " FOREIGN KEY (");
    strcat(sql_alter, field_name);
    strcat(sql_alter, ") REFERENCES graphql_proxy.");
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

hashmap *schema_convert(const char *json_schema, const char* file_types_reflection, 
        char *db_name, char *db_host, int db_port) {
    char *sql_create;
    char *sql_create_schema;
    char *sql_alter_queries[ALTER_QUERIES_NUMBER];
    size_t sql_alter_queries_num;
    size_t numEntries;
    ConfigEntry* configEntries;
    cJSON *json;
    cJSON *definitions;
    char conn_info[256];
    PGconn *conn;
    PGresult *res;
    int status;

    hashmap *resolvers = hashmap_create();
	if (resolvers == NULL) {
		printf("schema_convert(): failed while hashmap init\n");
		goto hashmap_create_fail;
	}

    snprintf(conn_info, 256, "dbname=%s host=%s port=%d", db_name, db_host, db_port);
    if (!create_connection(&conn, conn_info)) {
        goto create_connection_fail;
    }

    configEntries = load_config_file(file_types_reflection, &numEntries);
    if (configEntries == NULL) {
        goto load_config_file_fail;
    }

	// parse the JSON data
	json = cJSON_Parse(json_schema);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "schema_convert(): Parsing json schema failed.\n Error: %s %ld\n", error_ptr, error_ptr - json_schema);
		}
		goto json_parse_fail;
	}

    // query for drop and create schema
    sql_create_schema = (char *)calloc(QUERY_LENGTH, sizeof(char));
    if (sql_create_schema == NULL) {
        elog(LOG, "schema_convert(): sql_create_schema malloc failed\n");
        goto sql_create_schema_fail;
    }
    strcpy(sql_create_schema, "DROP SCHEMA IF EXISTS graphql_proxy CASCADE; CREATE SCHEMA graphql_proxy;");

    // execute create schema query
    status = exec_query(&conn, sql_create_schema, &res);
    if (status == 0) {
        goto exec_create_schema_fail;
    }
    clearRes(&res);

    sql_create = (char*)calloc(QUERY_LENGTH, sizeof(char));
    if (sql_create == NULL) {
        elog(LOG, "schema_convert(): sql_create malloc failed\n");
        goto sql_create_fail;
    }
    sql_alter_queries_num = 0;

	definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");

    types.numCreatedTypes = 0;
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
            if (strcmp(definition_name_value->valuestring, "Mutation") == 0 || (strcmp(definition_name_value->valuestring, "Query") == 0)) {
                // convert operation
                operation_convert(definition, resolvers, configEntries, numEntries);
                continue;
            }
        }

        
        strcpy(sql_create, "CREATE TABLE IF NOT EXISTS graphql_proxy.");
        type_name = cJSON_GetObjectItemCaseSensitive(definition, "name");
        type_name_value = cJSON_GetObjectItemCaseSensitive(type_name, "value");

        // found name of table
        if (cJSON_IsString(type_name_value) && (type_name_value->valuestring != NULL)) {
            elog(LOG, "schema_convert(): table[%d]: %s\n", i, type_name_value->valuestring);
            strcat(sql_create, type_name_value->valuestring);
            strcat(sql_create, "(");

            // add surrogate primary key to table
            strcat(sql_create, "pk_");
            strcat(sql_create, type_name_value->valuestring);
            strcat(sql_create, " SERIAL PRIMARY KEY");

            // add type to types array
            strcpy(types.createdTypes[types.numCreatedTypes++], type_name_value->valuestring);
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
                elog(LOG, "schema_convert(): \tfield[%d]: %s\n", j, field_name_value->valuestring);
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
                    
                    elog(LOG, "schema_convert(): \t\tfield_type[%d]: %s\n", j, field_type_type_name_value->valuestring);
                    convertedType = get_config_value(field_type_type_name_value->valuestring, configEntries, numEntries);
                    if (convertedType == NULL) {
                        if (is_type_exists(field_type_type_name_value->valuestring)) {
                            strcat(sql_create, "INT ");

                            sql_alter_queries[sql_alter_queries_num] = (char*)calloc(QUERY_LENGTH, sizeof(char));
                            if (sql_alter_queries[sql_alter_queries_num] == NULL) {
                                elog(LOG, "schema_convert(): sql_alter_query malloc failed\n");
                                goto sql_alter_malloc_fail;
                            }
                            create_foreign_key(sql_alter_queries[sql_alter_queries_num], 
                                             type_name_value->valuestring, 
                                             field_type_type_name_value->valuestring, 
                                             field_name_value->valuestring);

                            elog(LOG, "schema_convert(): sql_alter: %s\n", sql_alter_queries[sql_alter_queries_num]);
                            ++sql_alter_queries_num;
                        } else {
                            elog(LOG, "schema_convert(): Type is not found: %s\n", field_type_type_name_value->valuestring);
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
                        
                        elog(LOG, "schema_convert(): \t\tfield_type[%d]: List of %s\n", j, field_type_type_type_type_name_value->valuestring);
                        convertedType = get_config_value(field_type_type_type_type_name_value->valuestring, configEntries, numEntries);
                        if (convertedType == NULL) {
                            elog(LOG, "schema_convert(): Type is not found: %s\n", field_type_type_type_type_name_value->valuestring);
                        } else {
                            strcat(sql_create, convertedType);
                            strcat(sql_create, " ARRAY ");
                        }
                    }
                } else {
                    elog(LOG, "schema_convert(): Nested lists is not supported\n");
                    //free resources
                    //
                    return NULL;
                }
            }
            
            field_type_kind = cJSON_GetObjectItemCaseSensitive(field_type, "kind");

            // found kind of field
            if (cJSON_IsString(field_type_kind) && (field_type_kind->valuestring != NULL)) {
                elog(LOG, "schema_convert(): \t\tfield_kind[%d]: %s\n", j, field_type_kind->valuestring);
                if (strcmp(field_type_kind->valuestring, "NonNullType") == 0)
                    strcat(sql_create, "NOT NULL");
            }
        }
        strcat(sql_create, ");");
        elog(LOG, "schema_convert(): sql_create query: %s\n", sql_create);

        // execute create table query
        status = exec_query(&conn, sql_create, &res);
        if (status == 0) {
            goto exec_create_fail;
        }
        clearRes(&res);        

        for (size_t i = 0; i < sql_alter_queries_num; ++i) {
            status = exec_query(&conn, sql_alter_queries[i], &res);
            if (status == 0) {
                goto exec_alter_fail;
            }
            clearRes(&res);
        }

        memset(sql_create, 0, QUERY_LENGTH);
        free_alter_queries(sql_alter_queries, sql_alter_queries_num);
    }


    free(sql_create);
    cJSON_Delete(json);
    free_config(configEntries, numEntries);
    close_connection(&conn, &res);
    return resolvers;


exec_create_fail:
exec_alter_fail:
sql_alter_malloc_fail:
    free_sql_alter_queries(sql_alter_queries, sql_alter_queries_num);
    free(sql_create);
sql_create_fail:
exec_create_schema_fail:
    free(sql_create_schema);
sql_create_schema_fail:
    cJSON_Delete(json);
json_parse_fail:
    free_config(configEntries, numEntries);
    close_connection(&conn, &res);
create_connection_fail:
load_config_file_fail:
    free(resolvers);
hashmap_create_fail:
    return NULL;
}
