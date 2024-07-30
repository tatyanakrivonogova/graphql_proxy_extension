#include "query_parser.h"

#include "schema/schema.h"
#include <stdio.h>

Datum
my_run(PG_FUNCTION_ARGS) {
	// char *input = text_to_cstring(PG_GETARG_TEXT_PP(0));
}

char* to_sql(char *expr) {
    char* result = (char *)malloc(sizeof(char) * 300);
    char* head = (char *)malloc(sizeof(char) * 300);
    sscanf(expr, "%s{", result);
    return result;
}

char* handle_operation_query(const char *json_query, int fd) {
    char* response; 
    cJSON *json;
    cJSON *definitions;
    char *query_for_execution;
    json = cJSON_Parse(json_query);
    if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			elog(LOG, "Error: %s %ld\n", error_ptr, error_ptr - json_query);
		}
		cJSON_Delete(json);
		return NULL;
	}

    // access the JSON data
	definitions = cJSON_GetObjectItemCaseSensitive(json, "definitions");
    elog(LOG, "definitions: %p\n", definitions);
    if (cJSON_GetArraySize(definitions) != 1) {
        elog(LOG, "Request should consist one operation\n");
        return NULL;
    }

    elog(LOG, "definitions size: %d", cJSON_GetArraySize(definitions));
    for (int i = 0; i < cJSON_GetArraySize(definitions); ++i) {
        cJSON *definition;
        cJSON *kind;
        cJSON *operation;
        cJSON *selection_set;
        cJSON *selections_json;
        
        definition = cJSON_GetArrayItem(definitions, i);
        kind = cJSON_GetObjectItemCaseSensitive(definition, "kind");
        if (kind != NULL && (cJSON_IsString(kind)) && (kind->valuestring != NULL)
                && (strcmp(kind->valuestring, "OperationDefinition") != 0)) {
            elog(LOG, "Wrong query type\n");
            continue;
        }

        //operation type
        operation = cJSON_GetObjectItemCaseSensitive(definition, "operation");
        if (operation != NULL && (cJSON_IsString(operation)) && (operation->valuestring != NULL)
                && (strcmp(operation->valuestring, "mutation") != 0) && (strcmp(operation->valuestring, "query") != 0)) {
            elog(LOG, "Operation query or mutation expected\n");
            continue;
        }
        struct ArgValues args;
        struct Selections *selections = (struct Selection*)malloc(sizeof(struct Selection));
        selections->count = 0;
        selection_set = cJSON_GetObjectItemCaseSensitive(definition, "selectionSet");
        int depth = 0;
        parse_selection_set(selections, selection_set, depth);
        // log_stack(&selections);
        return "SELECT * from book;";
handle_operation_fail:
        elog(LOG, "handle_operation_fail\n");
        return NULL;
    }
    // return response;
}

void parse_selection_set(struct Selections* selections, cJSON *selection_set, int depth) {
    if (cJSON_GetArraySize(selection_set) == 0)
        return;
    cJSON *selections_json = cJSON_GetObjectItemCaseSensitive(selection_set, "selections");
    for (int j = 0; j < cJSON_GetArraySize(selections_json); ++j) {
        cJSON *selection_json;
        cJSON *selection_name;
        cJSON *selection_name_value;
        cJSON *selection_arguments;

        selection_json = cJSON_GetArrayItem(selections_json, j);
        struct Selection selection = parse_selection(selection_json, selections, depth);
        add_selection_struct(selections, &selection);
        log_stack(selections);
    }
    
}

struct Selection parse_selection(cJSON *selection, struct Selections* selections, int depth) {
    struct Selection result;
    cJSON* tmp = cJSON_GetObjectItemCaseSensitive(selection, "name");
    cJSON* name = cJSON_GetObjectItemCaseSensitive(tmp, "value");
    cJSON* arguments = cJSON_GetObjectItemCaseSensitive(selection, "arguments");
    strcpy(result.name, name->valuestring);
    result.argCount = cJSON_GetArraySize(arguments);
    for (int i = 0; i < result.argCount; i++) {
        tmp = cJSON_GetArrayItem(arguments, i);
        cJSON* tmp_name = cJSON_GetObjectItemCaseSensitive(tmp, "name");
        cJSON* argument_name = cJSON_GetObjectItemCaseSensitive(tmp_name, "value");
        strcpy(result.argName, argument_name->valuestring);
        tmp_name = cJSON_GetObjectItemCaseSensitive(tmp, "value");
        cJSON* argument_value = cJSON_GetObjectItemCaseSensitive(tmp_name, "value");
        strcpy(result.argValue, argument_value->valuestring);
    }
    result.depth = depth;
    cJSON* selection_set = cJSON_GetObjectItemCaseSensitive(selection, "selectionSet");
    int size = cJSON_GetArraySize(selection_set);
    if (size == 0) {
        elog(LOG, "selection set is null, selection name: %s, size: %d", result.name, size);
    } else {
        elog(LOG, "selection set is NOT null, selection name: %s, size: %d", result.name, size);
        parse_selection_set(selections, selection_set, depth + 1);
    }

    return result;
    // add_selection(selections, name->valuestring, arg_name, arg_value, args_count, depth);
}

void add_selection(struct Selections* selections, char* name, char* arg_name, char* arg_value, int arg_count, int depth) {
    size_t count = selections->count;
    strcpy(selections->selections[count].name, name);
    strcpy(selections->selections[count].argName, arg_name);
    strcpy(selections->selections[count].argValue, arg_value);
    selections->selections[count].argCount = arg_count;
    selections->selections[count].depth = depth;
    selections->count += 1;
}

void add_selection_struct(Selections *selections, Selection *selection) {

    size_t count = selections->count;
    strcpy(selections->selections[count].name, selection->name);
    strcpy(selections->selections[count].argName, selection->argName);
    strcpy(selections->selections[count].argValue, selection->argValue);
    selections->selections[count].argCount = selection->argCount;
    selections->selections[count].depth = selection->depth;
    selections->selections[count].is_selection_set = selection->is_selection_set;
    selections->count += 1;
    elog(LOG, "selection %d, name: %s, arg_count: %d, arg_name: %s, arg_value: %s, depth: %d, is_select_set: %d",
            999, selections->selections[count].name, selections->selections[count].argCount, selections->selections[count].argCount,
            selections->selections[count].argValue, selections->selections[count].depth, selections->selections[count].is_selection_set);
}

void log_stack(Selections *selections) {
    int count = selections->count;
    elog(LOG, "\nSELECTIONS STACK");
    for (int i = 0; i < count; i++) {
        elog(LOG, "selection №%d, name: %s, arg_count: %d, arg_name: %s, arg_value: %s, depth: %d, is_select_set: %d",
            count - i - 1, selections->selections[count].name, selections->selections[count].argCount, selections->selections[count].argCount,
            selections->selections[count].argValue, selections->selections[count].depth, selections->selections[count].is_selection_set);
    }
}
