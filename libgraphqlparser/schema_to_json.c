#include "schema_to_json.h"

#include "c/GraphQLAstNode.h"
#include "c/GraphQLParser.h"
#include "c/GraphQLAstToJSON.h"

#include "postgres.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Reading schema SDL and representation to json format for futher using.
 */

const char *schema_to_json(const char *schema_file) {
    const char *error;
    long fileSize;
    char* buffer;
    size_t bytesRead;
    struct GraphQLAstNode * AST;
    const char *json;

    // open file with schema
    FILE* file = fopen(schema_file, "rb");
    if (!file) {
        elog(LOG, "Open file %s failed\n", schema_file);
        return NULL;
    }

    // get file size
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    // allocate memory for schema
    buffer = (char*)malloc(fileSize + 1);
    if (!buffer) {
        elog(LOG, "Malloc for reading schema failed\n");
        fclose(file);
        return NULL;
    }

    // read schema
    bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        elog(LOG, "Reading schema failed\n");
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[fileSize] = '\0';
    fclose(file);

    // parse schema to AST
    AST = graphql_parse_string_with_experimental_schema_support((const char *)buffer, &error);
    if (!AST) {
        elog(LOG, "Parsing schema to AST failed: %s\n", error);
        free((void *)error);
        free(buffer);
        return NULL;
    }
    free(buffer);

    // parse AST to json
    json = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
    if (!json) {
        elog(LOG, "Parsing schema to json failed\n");
        graphql_node_free(AST);
        return NULL;
    }
    graphql_node_free(AST);

    return json;
}