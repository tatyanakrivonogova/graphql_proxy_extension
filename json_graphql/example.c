#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cJSON.h" 

#define NAME_LENGTH 63
#define MAX_TYPES_NUMBER 100
#define QUERY_LENGTH 256

typedef struct {
    char* key;
    char* value;
} ConfigEntry;

ConfigEntry* loadConfigFile(const char* filename, size_t* numEntries) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Open file failed\n");
        return NULL;
    }

    *numEntries = 0;
    int ch;
    while (EOF != (ch = fgetc(file))) {
        if (ch == '\n') {
            (*numEntries)++;
        }
    }
    rewind(file);

    ConfigEntry* entries = (ConfigEntry*)malloc((*numEntries) * sizeof(ConfigEntry));
    if (!entries) {
        fprintf(stderr, "Malloc failed\n");
        fclose(file);
        return NULL;
    }

    char line[100];
    for (size_t i = 0; i < *numEntries; i++) {
        if (!fgets(line, sizeof(line), file)) {
            fprintf(stderr, "File read failed\n");
            free(entries);
            fclose(file);
            return NULL;
        }

        line[strcspn(line, "\n")] = 0;

        char* separator = strchr(line, '=');
        if (!separator) {
            fprintf(stderr, "Invalid config parameter: %s\n", line);
            free(entries);
            fclose(file);
            return NULL;
        }

        *separator = '\0';

        entries[i].key = strdup(line);
        entries[i].value = strdup(separator + 1);
    }

    fclose(file);
    return entries;
}

char* getConfigValue(char *key, ConfigEntry *configEntries, size_t numEntries) {
    if (configEntries) {
        for (size_t i = 0; i < numEntries; i++)
            if (strcmp(key, configEntries[i].key) == 0) return configEntries[i].value;
        free(configEntries);
    }
    return NULL;
}

void freeConfig(ConfigEntry *configEntries, size_t numEntries) {
    if (configEntries) {
        for (size_t i = 0; i < numEntries; i++) {
            free(configEntries[i].key);
            free(configEntries[i].value);
        }
        free(configEntries);
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
	char buffer[4096]; 
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

    char *sql = (char*)calloc(QUERY_LENGTH, sizeof(char));
    strcpy(sql, "CREATE TABLE ");

	// access the JSON data 
	cJSON *definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions"); 
    printf("definitions: %p\n", definitions);
    for (int i = 0; i < cJSON_GetArraySize(definitions); ++i)
    {
        cJSON *definition = cJSON_GetArrayItem(definitions, i); 
        cJSON *kind_definition = cJSON_GetObjectItemCaseSensitive(definition, "kind"); 
        if (cJSON_IsString(kind_definition) && (kind_definition->valuestring != NULL) && (strcmp(kind_definition->valuestring, "ObjectTypeDefinition") != 0)) continue;
        cJSON *type_name = cJSON_GetObjectItemCaseSensitive(definition, "name"); 
        cJSON *type_name_value = cJSON_GetObjectItemCaseSensitive(type_name, "value"); 

        // found name of table
        if (cJSON_IsString(type_name_value) && (type_name_value->valuestring != NULL)) { 
            printf("table[%d]: %s\n", i, type_name_value->valuestring);
            strcat(sql, type_name_value->valuestring);
            strcat(sql, "(");
        } 

        cJSON *fields = cJSON_GetObjectItemCaseSensitive(definition, "fields");
        for (int j = 0; j < cJSON_GetArraySize(fields); ++j)
        {
            cJSON *field = cJSON_GetArrayItem(fields, j); 
            cJSON *kind_field = cJSON_GetObjectItemCaseSensitive(field, "kind"); 
            if (cJSON_IsString(kind_field) && (kind_field->valuestring != NULL) && (strcmp(kind_field->valuestring, "FieldDefinition") != 0)) continue;
            cJSON *field_name = cJSON_GetObjectItemCaseSensitive(field, "name"); 
            cJSON *field_name_value = cJSON_GetObjectItemCaseSensitive(field_name, "value"); 

            // found name of field
            if (cJSON_IsString(field_name_value) && (field_name_value->valuestring != NULL)) { 
                printf("\tfield[%d]: %s\n", j, field_name_value->valuestring); 
                strcat(sql, field_name_value->valuestring);
                strcat(sql, " ");
            } 

            cJSON *field_type = cJSON_GetObjectItemCaseSensitive(field, "type"); 
            cJSON *field_type_type = cJSON_GetObjectItemCaseSensitive(field_type, "type"); 
            cJSON *field_type_type_kind = cJSON_GetObjectItemCaseSensitive(field_type_type, "kind");
            if (field_type_type_kind != NULL && (cJSON_IsString(field_type_type_kind)) && (field_type_type_kind->valuestring != NULL) && (strcmp(field_type_type_kind->valuestring, "NamedType") == 0))
            {
                // NamedType
                cJSON *field_type_type_name = cJSON_GetObjectItemCaseSensitive(field_type_type, "name");
                cJSON *field_type_type_name_value = cJSON_GetObjectItemCaseSensitive(field_type_type_name, "value");

                // found type of fiend
                if (cJSON_IsString(field_type_type_name_value) && (field_type_type_name_value->valuestring != NULL)) { 
                    printf("\t\tfield_type[%d]: %s\n", j, field_type_type_name_value->valuestring); 
                    char *convertedType = getConfigValue(field_type_type_name_value->valuestring, configEntries, numEntries);
                    if (convertedType == NULL) {
                        printf("Type is not found: %s\n", field_type_type_name_value->valuestring);
                    } else {
                        // strcat(sql, field_type_type_name_value->valuestring);
                        strcat(sql, convertedType);
                        strcat(sql, " ");
                    }
                } 
            }
            else if (field_type_type_kind != NULL && (cJSON_IsString(field_type_type_kind)) && (field_type_type_kind->valuestring != NULL) && (strcmp(field_type_type_kind->valuestring, "ListType") == 0))
            {
                // ListType
                cJSON *field_type_type_type = cJSON_GetObjectItemCaseSensitive(field_type_type, "type"); 
                cJSON *field_type_type_type_type = cJSON_GetObjectItemCaseSensitive(field_type_type_type, "type"); 
                cJSON *field_type_type_type_type_kind = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type, "kind");
                if (field_type_type_type_type_kind != NULL && (cJSON_IsString(field_type_type_type_type_kind)) && (strcmp(field_type_type_type_type_kind->valuestring, "NamedType") == 0))
                {
                    // Lost of NNamedType
                    cJSON *field_type_type_type_type_name = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type, "name");
                    cJSON *field_type_type_type_type_name_value = cJSON_GetObjectItemCaseSensitive(field_type_type_type_type_name, "value");

                    // found type of field
                    if (field_type_type_type_type_name_value != NULL && (cJSON_IsString(field_type_type_type_type_name_value)) && (field_type_type_type_type_name_value->valuestring != NULL)) { 
                        printf("\t\tfield_type[%d]: List of %s\n", j, field_type_type_type_type_name_value->valuestring); 
                        char *convertedType = getConfigValue(field_type_type_type_type_name_value->valuestring, configEntries, numEntries);
                        if (convertedType == NULL) {
                            printf("Type is not found: %s\n", field_type_type_type_type_name_value->valuestring);
                        } else {
                            // strcat(sql, field_type_type_name_value->valuestring);
                            strcat(sql, convertedType);
                            strcat(sql, " ARRAY ");
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
                    strcat(sql, "NOT NULL");
            } 

            if (j != cJSON_GetArraySize(fields)-1) strcat(sql, ", ");
        }
        strcat(sql, ");");
        printf("sql query: %s\n", sql);
        memset(sql, 0, QUERY_LENGTH);
    }

	// delete the JSON object
	cJSON_Delete(json);
    freeConfig(configEntries, numEntries);
	return 0; 
}
