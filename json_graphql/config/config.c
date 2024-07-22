#include "config.h"

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