#include "../json_graphql/cJSON.h"
#include "../hashmap/map.h"
#include "../config/config.h"

void operation_convert(cJSON *definition, hashmap *resolvers, ConfigEntry* configEntries, size_t numEntries);