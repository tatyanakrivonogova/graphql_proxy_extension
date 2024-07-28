#include "config.h"

#include "postgres.h"

ConfigEntry* load_config_file(const char* filename, size_t* numEntries) {
    int ch;
    ConfigEntry* entries;
    char line[100];

    FILE* file = fopen(filename, "r");
    if (!file) {
        elog(LOG, "Open file failed\n");
        return NULL;
    }

    *numEntries = 0;
    while (EOF != (ch = fgetc(file))) {
        if (ch == '\n') {
            (*numEntries)++;
        }
    }
    rewind(file);

    entries = (ConfigEntry*)malloc((*numEntries) * sizeof(ConfigEntry));
    if (!entries) {
        elog(LOG, "Malloc failed\n");
        fclose(file);
        return NULL;
    }

    for (size_t i = 0; i < *numEntries; i++) {
        char* separator;
        if (!fgets(line, sizeof(line), file)) {
            elog(LOG, "File read failed\n");
            free(entries);
            fclose(file);
            return NULL;
        }

        line[strcspn(line, "\n")] = 0;

        separator = strchr(line, '=');
        if (!separator) {
            elog(LOG, "Invalid config parameter: %s\n", line);
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

char* get_config_value(char *key, ConfigEntry *configEntries, size_t numEntries) {
    if (configEntries) {
        for (size_t i = 0; i < numEntries; i++)
            if (strcmp(key, configEntries[i].key) == 0) return configEntries[i].value;
    }
    return NULL;
}

int get_int_value(char *value) {
    if (value == NULL) return 0;
    return atoi(value);
}

void free_config(ConfigEntry *configEntries, size_t numEntries) {
    if (configEntries) {
        for (size_t i = 0; i < numEntries; i++) {
            free(configEntries[i].key);
            free(configEntries[i].value);
        }
        free(configEntries);
    }
}