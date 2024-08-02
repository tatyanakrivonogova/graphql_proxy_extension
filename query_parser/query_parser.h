#include "postgres.h"
#include "handlers/utils.h"
#include "json_graphql/cJSON.h"

char* handle_operation_query(cJSON* definition);
void parse_selection(cJSON* selection, struct Selections* selections, int depth);
void parse_selection_set(struct Selections* selections, cJSON* selection_set, int depth);
void add_selection_struct(struct Selections* selections, struct Selection* selection);
void log_stack(struct Selections* selections);
char* make_sql_query(struct Selections* selections);
