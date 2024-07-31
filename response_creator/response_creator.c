#include "response_creator.h"

#include <string.h>
#include "postgres.h"

void create_data_response(char *buffer, char *response, int response_len, int *output_size) {
    elog(LOG, "create_data_response\n");
    *output_size = sizeof("HTTP/1.1 200 OK\nContent-Type: application/json\n\n") - 1 + response_len;
    strcpy(buffer, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n");
    strncpy(buffer + sizeof("HTTP/1.1 200 OK\nContent-Type: application/json\n\n") - 1, response, response_len);
}

void create_bad_request_response(char *buffer, int *output_size) {
    elog(LOG, "create_bad_request_response\n");
    *output_size = sizeof("HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Bad request\"\n    }\n  ]\n}\n");
    strcpy(buffer, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Bad request\"\n    }\n  ]\n}\n");
}

void create_server_error_response(char *buffer, int *output_size) {
    elog(LOG, "create_server_error_response\n");
    *output_size = sizeof("HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Server error\"\n    }\n  ]\n}");
    strcpy(buffer, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\n  \"errors\": [\n    {\n      \"message\": \"Server error\"\n    }\n  ]\n}");
}