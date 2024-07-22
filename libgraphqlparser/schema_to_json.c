#include "schema_to_json.h"

#include "c/GraphQLAstNode.h"
#include "c/GraphQLParser.h"
#include "c/GraphQLAstToJSON.h"

#include "postgres.h"

#include <stdio.h>
#include <stdlib.h>


char *schema_to_json() {
  const char *error;
  FILE* file = fopen("../contrib/graphql_proxy/libgraphqlparser/schema.graphql", "rb");
  if (!file) {
      elog(LOG, "Open file failed schema_to_json\n");
      return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1); // +1 для нулевого символа
  if (!buffer) {
      elog(LOG, "Malloc failed\n");
      fclose(file);
      return NULL;
  }

  size_t bytesRead = fread(buffer, 1, fileSize, file);
  if (bytesRead != fileSize) {
      elog(LOG, "Ошибка чтения файла\n");
      free(buffer);
      fclose(file);
      return NULL;
  }

  buffer[fileSize] = '\0';
  fclose(file);

  struct GraphQLAstNode * AST = graphql_parse_string_with_experimental_schema_support((const char *)buffer, &error);
  if (!AST) {
    printf("Parser failed with error: %s\n", error);
    free((void *)error);  // NOLINT
    return NULL;
  }

  const char *json = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
//   puts(json);
//   free((void *)json);  // NOLINT

  return json;
}