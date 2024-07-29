#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} ConfigEntry;

ConfigEntry* load_config_file(const char* filename, size_t* numEntries);

char* get_config_value(char *key, ConfigEntry *configEntries, size_t numEntries);

int get_int_value(char *value);

void free_config(ConfigEntry *configEntries, size_t numEntries);