#include "schema_to_json.h"

#include "c/GraphQLAstNode.h"
#include "c/GraphQLParser.h"
#include "c/GraphQLAstToJSON.h"

#include "postgres.h"

#include <stdio.h>
#include <stdlib.h>

const char *schema_to_json() {
  const char *error;
  long fileSize;
  char* buffer;
  size_t bytesRead;
  struct GraphQLAstNode * AST;
  const char *json;

  FILE* file = fopen("../contrib/graphql_proxy/libgraphqlparser/schema.graphql", "rb");
  if (!file) {
      elog(LOG, "Open file failed schema_to_json\n");
      return NULL;
  }

  fseek(file, 0, SEEK_END);
  fileSize = ftell(file);
  rewind(file);

  buffer = (char*)malloc(fileSize + 1);
  if (!buffer) {
      elog(LOG, "Malloc failed\n");
      fclose(file);
      return NULL;
  }

  bytesRead = fread(buffer, 1, fileSize, file);
  if (bytesRead != fileSize) {
      elog(LOG, "Read file error\n");
      free(buffer);
      fclose(file);
      return NULL;
  }

  buffer[fileSize] = '\0';
  fclose(file);

  AST = graphql_parse_string_with_experimental_schema_support((const char *)buffer, &error);
  if (!AST) {
    printf("Parser failed with error: %s\n", error);
    free((void *)error);
    return NULL;
  }

  json = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
  graphql_node_free(AST);
  free(buffer);
//   puts(json);
//   free((void *)json);

  return json;
}