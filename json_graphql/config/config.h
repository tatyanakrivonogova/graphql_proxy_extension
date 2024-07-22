#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} ConfigEntry;

ConfigEntry* loadConfigFile(const char* filename, size_t* numEntries);

char* getConfigValue(char *key, ConfigEntry *configEntries, size_t numEntries);

void freeConfig(ConfigEntry *configEntries, size_t numEntries);