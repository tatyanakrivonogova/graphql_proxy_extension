#include "../json_graphql/cJSON.h"
#include "../hashmap/map.h"
#include "../config/config.h"
#include "../postgres_connect/postgres_connect.h"

extern PGconn *pg_conn;

void operation_convert(cJSON *definition, hashmap *resolvers, 
        ConfigEntry* configEntries, size_t numEntries, char *resolvers_filename);