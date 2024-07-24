#include "../hashmap/map.h"

#define MAX_QUERY_LENGTH 256

void swap(char **a, char **b);
void insert_int(char *buffer, size_t buffer_size, const char *format, const char *value);
void insert_string(char *buffer, size_t buffer_size, const char *format, const char *value);

void handle_mutation(const char *json_query, hashmap *resolvers);