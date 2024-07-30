#include "input_parsing.h"

#include "../hashmap/map.h"
#include "../io_uring/event_handling.h"
#include "../io_uring/multiple_user_access.h"
#include "../postgres_connect/postgres_connect.h"
#include "http_parser.h"
#include "../handlers/handlers.h"
#include "../response_creator/response_creator.h"
#include "../libgraphqlparser/c/GraphQLAstNode.h"
#include "../libgraphqlparser/c/GraphQLParser.h"
#include "../libgraphqlparser/c/GraphQLAstToJSON.h"
#include "../query_parser/query_parser.h"

#include "postgres.h"

#include <string.h>
#include <stdlib.h>

int check_http_request(char *parsed_method, size_t method_len, char *parsed_path, 
                       size_t path_len, int minor_version) {
    if (strncmp(parsed_method, "POST", method_len) != 0) return -1;
    if (strncmp(parsed_path, "/query", path_len) != 0) return -1;
    if (minor_version != 1) return -1;
    return 0;
}

void
parse_input(char* buffer, size_t request_len, int* output_size, int fd, hashmap *resolvers) {
    const char *method;
    size_t method_len;
    const char *path;
    size_t path_len;
    int minor_version;
    size_t num_headers;
    struct phr_header headers[NUM_HEADERS];

    char* parsed_method = NULL;
    char* parsed_path = NULL;
    char* parsed_header_name = NULL;
    char* parsed_header_value = NULL;

    char* query_begin = NULL;
    char *query = NULL;
    char *response = NULL;
    size_t query_len;
    int err;
    const char *error = NULL;
    struct GraphQLAstNode * AST = NULL;
    const char *json_query = NULL;

    *output_size = 0;

    elog(LOG, "parse_input(): request: %.*s\n", (int)request_len, buffer);
    num_headers = NUM_HEADERS;
    err = phr_parse_request(buffer, request_len, &method, &method_len, &path, &path_len, &minor_version, headers, &num_headers, 0);
    if (err == -1 || err == -2) {
        elog(LOG, "parse_input(): PArse request failed\n");
        create_response_400(buffer, output_size);
        return;
    }

    parsed_method = (char *)malloc(method_len + 1);
    if (!parsed_method) goto parsed_method_malloc_fail;
    parsed_path = (char *)malloc(path_len + 1);
    if (!parsed_path) goto parsed_path_malloc_fail;
    parsed_method[method_len] = '\0';
    parsed_path[path_len] = '\0';

    strncpy(parsed_method, method, method_len);
    strncpy(parsed_path, path, path_len);

    elog(LOG, "parse_input(): parsed method: %ld %s\n", method_len, parsed_method);
    elog(LOG, "parse_input(): parsed path: %ld %s\n", path_len, parsed_path);
    elog(LOG, "parse_input(): parsed minor version: %d\n", minor_version);
    elog(LOG, "parse_input(): parsed num headers: %ld\n", num_headers);

    if (check_http_request(parsed_method, method_len, parsed_path, 
                       path_len, minor_version) == -1) {
        elog(LOG, "parse_input(): HTTP-request is not supported\n");
        create_response_400(buffer, output_size);
        goto unsupported_request;
    }
        
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

    // to do: add check http-headers
    //

    // parsing query
    if ((query_begin = strstr(buffer, "\n\n")) == 0) {
        elog(LOG, "parse_input(): query is empty\n");
        query_len = 0;
    } else {
        // save query
        query_len = buffer + request_len - query_begin - 2; // 2 is \n\n length
        query = (char *)malloc(query_len + 1);
        if (!query) goto query_malloc_fail;
        query[query_len] = '\0';
        strncpy(query, query_begin + 2, query_len); // 2 is \n\n length
        elog(LOG, "parse_input(): query_len: %ld query: %s\n", query_len, query);
    }


    if (query_len != 0) {
        AST = graphql_parse_string_with_experimental_schema_support((const char *)query, &error); // replace method
        if (!AST) {
            printf("parse_input(): Parsing query to AST failed with error: %s\n", error);
            free((void *)error);
            create_response_500(buffer, output_size);
            goto free_memory;
        }

        json_query = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
        if (!json_query) {
            printf("parse_input(): Parsing query to json failed\n");
            create_response_500(buffer, output_size);
            graphql_node_free(AST);
            goto free_memory;
        }
        elog(LOG, "parse_input(): parsed json query: %s\n", json_query);

        // handle operation and get response
        response = handle_operation_query(json_query, fd);
        // response = handle_operation(json_query, resolvers, fd);
        elog(LOG, "response after handle_operation_query: %s", response);
        // copy response for sending
        if (response != NULL) {
            elog(LOG, "parse_input(): response_len: %ld response: %s\n", strlen(response), response);
            create_response_200(buffer, response, strlen(response), output_size);
            free(response);
            response = NULL;
        } else {
            // send error
            create_response_500(buffer, output_size);
            response = NULL;
        }

        graphql_node_free(AST);
        free((void *)json_query);

    } else {
        // send http ok for empty request
        create_response_200(buffer, "", 0, output_size);
        response = NULL;
    }


free_memory:
    if (response) free(response);
query_malloc_fail:
    if (parsed_header_value) free(parsed_header_value);
parsed_header_value_malloc_fail:
    if (parsed_header_name) free(parsed_header_name);
parsed_header_name_malloc_fail:
unsupported_request:
    if (parsed_path) free(parsed_path);
parsed_path_malloc_fail:
    if (parsed_method) free(parsed_method);
parsed_method_malloc_fail:
    return;
}