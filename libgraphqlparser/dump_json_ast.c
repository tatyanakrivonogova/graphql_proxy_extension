/**
 * Copyright 2019-present, GraphQL Foundation
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "c/GraphQLAstNode.h"
#include "c/GraphQLParser.h"
#include "c/GraphQLAstToJSON.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
  const char *error;
  FILE* file = fopen("graphql.schema", "rb");
  if (!file) {
      fprintf(stderr, "Open file failed\n");
      return 1;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1); // +1 для нулевого символа
  if (!buffer) {
      fprintf(stderr, "Malloc failed\n");
      fclose(file);
      return 1;
  }

  size_t bytesRead = fread(buffer, 1, fileSize, file);
  if (bytesRead != fileSize) {
      fprintf(stderr, "Ошибка чтения файла\n");
      free(buffer);
      fclose(file);
      return 1;
  }

  buffer[fileSize] = '\0';
  fclose(file);

  struct GraphQLAstNode * AST = graphql_parse_string_with_experimental_schema_support((const char *)buffer, &error);
  if (!AST) {
    printf("Parser failed with error: %s\n", error);
    free((void *)error);  // NOLINT
    return 1;
  }

  const char *json = graphql_ast_to_json((const struct GraphQLAstNode *)AST);
  puts(json);
  free((void *)json);  // NOLINT

  return 0;
}