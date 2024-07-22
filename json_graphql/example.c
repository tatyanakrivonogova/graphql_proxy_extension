#include <stdbool.h>
#include "cJSON.h" 
#include "config/config.h"

#define NAME_LENGTH 63
#define MAX_TYPES_NUMBER 20
#define MAX_QUERIES_NUMBER 20
#define MAX_MUTATIONS_NUMBER 20
#define QUERY_LENGTH 256
#define ALTER_QUERIES_NUMBER 10


// types which are already converted to PostgresQL
typedef struct {
    size_t numCreatedTypes;
    char createdTypes[MAX_TYPES_NUMBER][NAME_LENGTH];
} Types;
Types types;


typedef struct {
    size_t numCreatedQueries;
    char createdQueries[MAX_QUERIES_NUMBER][NAME_LENGTH];
} Queries;
Queries queries;


typedef struct {
    size_t numCreatedMutations;
    char createdMutations[MAX_MUTATIONS_NUMBER][NAME_LENGTH];
} Mutations;
Mutations mutations;


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


int main() { 
    const char* filename = "config.txt";
    size_t numEntries;
    ConfigEntry* configEntries = loadConfigFile(filename, &numEntries);

	// open the file
	FILE *fp = fopen("data.json", "r");
	if (fp == NULL) {
		printf("Error: Unable to open the file.\n");
        freeConfig(configEntries, numEntries);
		return 1;
	}

	// read the file contents into a string
	char buffer[10000];
	int len = fread(buffer, 1, sizeof(buffer), fp);
	fclose(fp);

	// parse the JSON data
	cJSON *json = cJSON_Parse(buffer);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			printf("Error: %s %ld\n", error_ptr, error_ptr - buffer);
		}
		cJSON_Delete(json);
        freeConfig(configEntries, numEntries);
		return 1;
	}

    char *sql_create = (char*)calloc(QUERY_LENGTH, sizeof(char));
    char *sql_alter_queries[ALTER_QUERIES_NUMBER];
    size_t sql_alter_queries_num = 0;

	// access the JSON data
	cJSON *definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    printf("definitions: %p\n", definitions);

    types.numCreatedTypes = 0;
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
                printf("Query\n");
                // load sql-code for this query
                continue;
            } else if (strcmp(definition_name_value->valuestring, "Mutation") == 0) {
                printf("Mutation\n");
                // load sql-code for this mutation
                continue;
            }
        }

        
        strcpy(sql_create, "CREATE TABLE ");
        cJSON *type_name = cJSON_GetObjectItemCaseSensitive(definition, "name");
        cJSON *type_name_value = cJSON_GetObjectItemCaseSensitive(type_name, "value");

        // found name of table
        if (cJSON_IsString(type_name_value) && (type_name_value->valuestring != NULL)) {
            printf("table[%d]: %s\n", i, type_name_value->valuestring);
            strcat(sql_create, type_name_value->valuestring);
            strcat(sql_create, "(");

            // add surrogate primary key to table
            strcat(sql_create, "pk_");
            strcat(sql_create, type_name_value->valuestring);
            strcat(sql_create, " SERIAL PRIMARY KEY");

            // add type to types array
            strcpy(types.createdTypes[types.numCreatedTypes++], type_name_value->valuestring);
            printf("--- types number: %ld\n", types.numCreatedTypes);
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
                printf("\tfield[%d]: %s\n", j, field_name_value->valuestring);
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
                    printf("\t\tfield_type[%d]: %s\n", j, field_type_type_name_value->valuestring);
                    char *convertedType = getConfigValue(field_type_type_name_value->valuestring, configEntries, numEntries);
                    if (convertedType == NULL) {
                        if (isTypeExists(field_type_type_name_value->valuestring)) {
                            strcat(sql_create, "INT ");

                            sql_alter_queries[sql_alter_queries_num] = (char*)calloc(QUERY_LENGTH, sizeof(char));
                            createForeignKey(sql_alter_queries[sql_alter_queries_num], 
                                             type_name_value->valuestring, 
                                             field_type_type_name_value->valuestring, 
                                             field_name_value->valuestring);

                            printf("sql_alter: %s\n", sql_alter_queries[sql_alter_queries_num]);
                            ++sql_alter_queries_num;
                        } else {
                            printf("Type is not found: %s\n", field_type_type_name_value->valuestring);
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
                        printf("\t\tfield_type[%d]: List of %s\n", j, field_type_type_type_type_name_value->valuestring);
                        char *convertedType = getConfigValue(field_type_type_type_type_name_value->valuestring, configEntries, numEntries);
                        if (convertedType == NULL) {
                            printf("Type is not found: %s\n", field_type_type_type_type_name_value->valuestring);
                        } else {
                            strcat(sql_create, convertedType);
                            strcat(sql_create, " ARRAY ");
                        }
                    }
                } else {
                    printf("Nested lists is not supported\n");
                    return 1;
                }
            }
            
            cJSON *field_type_kind = cJSON_GetObjectItemCaseSensitive(field_type, "kind");

            // found kind of field
            if (cJSON_IsString(field_type_kind) && (field_type_kind->valuestring != NULL)) {
                printf("\t\tfield_kind[%d]: %s\n", j, field_type_kind->valuestring);
                if (strcmp(field_type_kind->valuestring, "NonNullType") == 0)
                    strcat(sql_create, "NOT NULL");
            }
        }
        strcat(sql_create, ");");
        printf("sql_create query: %s\n", sql_create);

        memset(sql_create, 0, QUERY_LENGTH);
        freeAlterQueries(sql_alter_queries, sql_alter_queries_num);
    }

    free(sql_create);
    freeConfig(configEntries, numEntries);
	// delete the JSON object
	cJSON_Delete(json);
	return 0;
}
