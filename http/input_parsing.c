#include "input_parsing.h"

#include "../io_uring/event_handling.h"
#include "http_parser.h"
#include "libgraphqlparser/c/GraphQLAstNode.h"
#include "libgraphqlparser/c/GraphQLParser.h"
#include "libgraphqlparser/c/GraphQLAstToJSON.h"

#include "postgres.h"

#include <string.h>
#include <stdlib.h>

void test_connect(void);

void
parse_input(char* request, size_t request_len, int* outputSize, int fd) {
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
    size_t query_len;
    int err;
    const char *error;
    struct GraphQLAstNode * AST;
    const char *json;

    *outputSize = 0;

     //example connection
    test_connect();

    elog(LOG, "read from client: %ld\n", request_len);
    num_headers = NUM_HEADERS;
    err = phr_parse_request(request, request_len, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    if (err == -1 || err == -2) {
        printf("send_request_to_server(): failed while parse HTTP request, error %d\n", err);
        *outputSize = sizeof("Failed while parse HTTP resuest. Change and try again\n");
        strcpy((char*)&bufs[fd], "Failed while parse HTTP request. Change and try again\n");
        // res = write(io_handle->fd, "Failed while parse HTTP request. Change and try again\n", 
        //             sizeof("Failed while parse HTTP resuest. Change and try again\n"));
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
    if ((query_begin = strstr(request, "\n\n")) == 0) {
        elog(LOG, "query is empty\n");
        query_len = 0;
    } else {
        query_len = request + request_len - query_begin - 2;
        query = (char*)malloc(query_len+1);
        if (!query) goto query_malloc_fail;
        query[query_len] = '\0';
        strncpy(query, query_begin + 2, query_len);
        elog(LOG, "query_len: %ld query: %s\n", query_len, query);
        strcpy(bufs[fd], query);
        *outputSize = query_len;
    }
    //res = write(io_handle->fd, query, (size_t)query_len);
    elog(LOG, "buffer after query pars: %s", (char*)&bufs[fd]);

    AST = graphql_parse_string_with_experimental_schema_support((const char *)query, &error);
    if (!AST) {
        printf("Parser failed with error: %s\n", error);
        free((void *)error);  // NOLINT
        goto free_memory;
    }

    json = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
    elog(LOG, "parsed json schema: %s\n", json);
    free((void *)json);

    // close connection after completing request

free_memory:
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