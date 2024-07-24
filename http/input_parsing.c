#include "input_parsing.h"

#include "../hashmap/map.h"
#include "../io_uring/event_handling.h"
#include "io_uring/multiple_user_access.h"
#include "postgres_connect/postgres_connect.h"
#include "http_parser.h"
#include "../handlers/handlers.h"
#include "../libgraphqlparser/c/GraphQLAstNode.h"
#include "../libgraphqlparser/c/GraphQLParser.h"
#include "../libgraphqlparser/c/GraphQLAstToJSON.h"

#include "postgres.h"

#include <string.h>
#include <stdlib.h>

void
parse_input(char* request, size_t request_len, int* outputSize, int fd, hashmap *resolvers) {
    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    size_t num_headers;
    struct phr_header headers[NUM_HEADERS];

    char* parsed_method;
    char* parsed_path;
    char* parsed_header_name = NULL;
    char* parsed_header_value = NULL;

    char* query_begin;
    char *query = NULL;
    char *response = NULL;
    size_t query_len;
    size_t response_len;
    int err;
    const char *error;
    struct GraphQLAstNode * AST;
    const char *json_query;
    int index;

    *outputSize = 0;

     //example connection
    // test_connect();

    elog(LOG, "read from client: %ld\n", request_len);
    elog(LOG, "request: %.*s\n", (int)request_len, request);
    num_headers = NUM_HEADERS;
    err = phr_parse_request(request, request_len, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    if (err == -1 || err == -2) {
        *outputSize = sizeof("Failed while parse HTTP resuest. Change and try again\n");
        strcpy((char*)&bufs[fd], "Failed while parse HTTP request. Change and try again\n");
        goto write_fail_response;
    }

    parsed_method = (char*)malloc(method_len + 1);
    if (!parsed_method) goto parsed_method_malloc_fail;
    parsed_path = (char*)malloc(path_len + 1);
    if (!parsed_path) goto parsed_path_malloc_fail;
    parsed_method[method_len] = '\0';
    parsed_path[path_len] = '\0';

    strncpy(parsed_method, method, method_len);
    strncpy(parsed_path, path, path_len);

    elog(LOG, "parsed method: %ld %s\n", method_len, parsed_method);
    elog(LOG, "parsed path: %ld %s\n", path_len, parsed_path);
    elog(LOG, "parsed minor version: %d\n", minor_version);
    elog(LOG, "parsed num headers: %ld\n", num_headers);
        
    // parsing of headers
    for (size_t i = 0; i < num_headers; ++i) {
        parsed_header_name = (char*)malloc(headers[i].name_len + 1);
        if (!parsed_header_name) goto parsed_header_name_malloc_fail;
        parsed_header_value = (char*)malloc(headers[i].value_len + 1);
        if (!parsed_header_value) goto parsed_header_value_malloc_fail;
        parsed_header_name[headers[i].name_len] = '\0';
        parsed_header_value[headers[i].value_len] = '\0';
        strncpy(parsed_header_name, headers[i].name, headers[i].name_len);
        strncpy(parsed_header_value, headers[i].value, headers[i].value_len);
        elog(LOG, "parsed header: %s: %s\n", parsed_header_name, parsed_header_value);
    }

    // parsing query
    if (((query_begin = strstr(request, "\n\n")) == 0) && ((query_begin = strstr(request, "\r\n\r\n")) == 0)) {
        elog(LOG, "query is empty\n");
        query_len = 0;
    } else {
        char *response_begin = "HTTP/1.1 200 OK\nContent-Type: text/plain;charset=UTF-8\n\n";
        
        // save query
        query_len = request + request_len - query_begin - 2;
        query = (char*)malloc(query_len+1);
        if (!query) goto query_malloc_fail;
        query[query_len] = '\0';
        strncpy(query, query_begin + 2, query_len);
        elog(LOG, "query_len: %ld query: %s\n", query_len, query);

                // save response
        response_len = query_len + strlen(response_begin);
        response = (char*)malloc(response_len+1);
        if (!response) goto response_malloc_fail;
        response[response_len] = '\0';
        strncpy(response, response_begin, strlen(response_begin));
        strncpy(response + strlen(response_begin), query_begin + 2, query_len);
        elog(LOG, "response_len: %ld response: %s\n", response_len, response);
        strncpy(bufs[fd], response, response_len);
        *outputSize = response_len;
    }
    elog(LOG, "buffer after query pars: %.*s\n", (int)query_len, query);


    if (query_len != 0) {
        AST = graphql_parse_string_with_experimental_schema_support((const char *)query, &error);
        if (!AST) {
            printf("Parser failed with error: %s\n", error);
            free((void *)error);
            goto free_memory;
        }

        json_query = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
        elog(LOG, "parsed json query: %s\n", json_query);
        handle_mutation(json_query, resolvers);
        free((void *)json_query);
    }

    //test sql query execution
    // if (get_conn_index(fd, &index)) {
    //     elog(LOG, "try to exec row sql query");
    //     exec_query(&conns[index].pg_conn, query, &conns[index].pg_res);
    // }

    // close connection after completing request

free_memory:
    if (response) free(response);
response_malloc_fail:
    if (query) free(query);
query_malloc_fail:
    if (parsed_header_value) free(parsed_header_value);
parsed_header_value_malloc_fail:
    if (parsed_header_name) free(parsed_header_name);
parsed_header_name_malloc_fail:
    free(parsed_path);
parsed_path_malloc_fail:
    free(parsed_method);
parsed_method_malloc_fail:
write_fail_response:
    elog(LOG, "memory is free\n");
    return;
}