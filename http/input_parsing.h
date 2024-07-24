#include <stddef.h>
#include "../hashmap/map.h"

#define NUM_HEADERS             (100)

void
parse_input(char* request, size_t request_len, int* outputSize, int fd, hashmap *resolvers);