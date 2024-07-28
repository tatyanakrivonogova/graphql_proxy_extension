#include "../json_graphql/cJSON.h"
#include "../hashmap/map.h"
#include "../json_graphql/config/config.h"

void operation_convert(cJSON *definition, hashmap *resolvers, ConfigEntry* configEntries, size_t numEntries);