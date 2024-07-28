#include "response_creator.h"

#include <string.h>
#include "postgres.h"

void create_response_200(char *buffer, char *response, int response_len, int *output_size) {
    elog(LOG, "create_response_200\n");
    *output_size = sizeof("HTTP/1.1 200 OK\nContent-Type: application/json\n\n") - 1 + response_len;
    strcpy(buffer, "HTTP/1.1 200 OK\nContent-Type: application/json\n\n");
    strncpy(buffer + sizeof("HTTP/1.1 200 OK\nContent-Type: application/json\n\n") - 1, response, response_len);
}

void create_response_400(char *buffer, int *output_size) {
    elog(LOG, "create_response_400\n");
    *output_size = sizeof("HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nFailed while parse HTTP resuest. Change and try again\n");
    strcpy(buffer, "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\nFailed while parse HTTP resuest. Change and try again\n");
}

void create_response_500(char *buffer, int *output_size) {
    elog(LOG, "create_response_500\n");
    *output_size = sizeof("HTTP/1.1 500 Internal Server Error\nContent-Type: application/json\n\n");
    strcpy(buffer, "HTTP/1.1 500 Internal Server Error\nContent-Type: application/json\n\n");
}