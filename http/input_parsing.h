#include <stddef.h>
#include "../hashmap/map.h"

#define NUM_HEADERS             (100)

int 
check_http_request(char *parsed_method, size_t method_len, char *parsed_path, 
                       size_t path_len, int minor_version);

void
parse_input(char* request, size_t request_len, int* outputSize, int fd, hashmap *resolvers);