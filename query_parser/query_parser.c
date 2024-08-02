#include "query_parser.h"

#include "schema/schema.h"
#include <stdio.h>

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
        memset(selections, 0, sizeof(selections));
        elog(LOG, "selection size: %d", sizeof(struct Selections));
        selection_set = cJSON_GetObjectItemCaseSensitive(definition, "selectionSet");
        int depth = 0;
        parse_selection_set(selections, selection_set, depth);
        log_stack(selections);
        response = make_sql_query(selections);
        elog(LOG, "sql query: %s", response);
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
        // struct Selection* selection;
        // if () {
        //     add_selection_struct(selections, selection);
        // }
        parse_selection(selection_json, selections, depth);
    }
    log_stack(selections);
    return;
}

void parse_selection(cJSON *selection, struct Selections* selections, int depth) {
    struct Selection* result = (struct Selection *)malloc(sizeof(struct Selection));
    memset(result, 0, sizeof(result));
    cJSON* tmp = cJSON_GetObjectItemCaseSensitive(selection, "name");
    cJSON* name = cJSON_GetObjectItemCaseSensitive(tmp, "value");
    cJSON* arguments = cJSON_GetObjectItemCaseSensitive(selection, "arguments");
    strcpy(result->name, name->valuestring);
    result->argCount = cJSON_GetArraySize(arguments);
    for (int i = 0; i < result->argCount; i++) {
        tmp = cJSON_GetArrayItem(arguments, i);
        cJSON* tmp_name = cJSON_GetObjectItemCaseSensitive(tmp, "name");
        cJSON* argument_name = cJSON_GetObjectItemCaseSensitive(tmp_name, "value");
        elog(LOG, "argument_name: %s",argument_name->valuestring);
        strcpy(result->argName, argument_name->valuestring);
        tmp_name = cJSON_GetObjectItemCaseSensitive(tmp, "value");
        cJSON* argument_value = cJSON_GetObjectItemCaseSensitive(tmp_name, "value");
        elog(LOG, "argument_value: %s",argument_value->valuestring);
        strcpy(result->argValue, argument_value->valuestring);
    }
    result->depth = depth;
    cJSON* selection_set = cJSON_GetObjectItemCaseSensitive(selection, "selectionSet");
    int size = cJSON_GetArraySize(selection_set);
    if (size == 0) {
        result->is_selection_set = false;
        elog(LOG, "selection set is null, selection name: %s, size: %d", result->name, size);
        add_selection_struct(selections, result);
        // return 1;
    } else {
        result->is_selection_set = true;
        result->depth += 1;
        elog(LOG, "selection set is NOT null, selection name: %s, size: %d", result->name, size);
        add_selection_struct(selections, result);
        parse_selection_set(selections, selection_set, depth + 1);
        // return 0;
    }

    // return result;
    // add_selection(selections, name->valuestring, arg_name, arg_value, args_count, depth);
}

void add_selection_struct(Selections *selections, Selection *selection) {
    elog(LOG, "add selection struct");
    size_t count = selections->count;
    selections->selections[count] = selection;
    selections->count += 1;
}

void log_stack(Selections *selections) {
    int count = selections->count;
    elog(LOG, "\nSELECTIONS STACK, size: %d", count);
    for (int i = count - 1; i >= 0; i--) {
        elog(LOG, "selection: %d, name: %s, arg_count: %d, arg_name: %s, arg_value: %s, depth: %d, is_select_set: %d",
            i, selections->selections[i]->name, selections->selections[i]->argCount, selections->selections[i]->argName,
            selections->selections[i]->argValue, selections->selections[i]->depth, selections->selections[i]->is_selection_set);
    }
}

char* make_sql_query(struct Selections* selections) {
    char* query = (char *)malloc(sizeof(char) * 1024);
    memset(query, 0, sizeof(query));
    size_t count = selections->count;
    int max_depth_index = get_deepest(selections);
    form_query_begin(query, max_depth_index, selections);
    elog(LOG, "make query begin: %s", query);
    // form_layer_query(query, selections, max_depth_index);
    //todo: selections->selections[count - 1] - main table name
    int i = 1;
    while (max_depth_index > 0) {
        elog(LOG, "got deepest");
        max_depth_index = get_deepest(selections);
        if (max_depth_index == count) {
            return query;
        }
        form_layer_query(query, selections, max_depth_index, i);
        i++;
    }
    elog(LOG, "query: \n%s", query);
    return query;
}

void form_layer_query(char* query, struct Selections* selections, int layer_index, int itteration) {
    strcat(query, "\n\tLATERAL (\n");
    char* table_name = (char *)malloc(sizeof(char) * NAME_LENGTH);
    // 1 - dot size
    char* tmp = (char *)malloc(sizeof(char) * (NAME_LENGTH * 2 + 1));
    sprintf(table_name, "%s", selections->selections[layer_index]->name);
    strcat(query, "\t\tSELECT");
    int depth = selections->selections[layer_index]->depth;
    disable_selection(selections, layer_index);
    int cur_index = layer_index + 1;
    while (cur_index < selections->count) {
        if (cur_index == layer_index + 1) {
            sprintf(tmp, " %s.%s", table_name, selections->selections[cur_index]->name);
        }
        else {
            sprintf(tmp, ", %s.%s", table_name, selections->selections[cur_index]->name);
        }
        strcat(query, tmp);
        // elog(LOG, "query: %s", query);
        elog(LOG, "%s", tmp);
        memset(tmp, 0, sizeof(tmp));
        disable_selection(selections, cur_index);
        cur_index = get_next_selection_index(selections, cur_index, depth);
    }
    add_previous_subquery(query, itteration);
    sprintf(tmp, " FROM %s\n\t) AS \"sub/%d\"", table_name, itteration);
    strcat(query, tmp);
    add_final_symbol(query, depth);
    elog(LOG, "query: %s", query);
    free(tmp);
}

void add_previous_subquery(char* query, int itteration) {
    if (itteration == 1) {
        return;
    }
    char* sub = (char *)malloc(sizeof(char) * NAME_LENGTH);
    sprintf(sub, ", \"sub/%d\"", itteration - 1);
    strcat(query, sub);
    free(sub);
}

void add_final_symbol(char* query, int depth) {;
    if (depth == 1) {
        strcat(query, ";");
    } else {
        strcat(query, ",");
    }
}

int get_next_selection_index(struct Selections* selections, int cur_index, int depth) {
    while (1) {
        cur_index += 1;
        elog(LOG, "cur_index in while: %d", cur_index);
        if (cur_index >= selections->count) {
            elog(LOG, "cur_index is invalid, return");
            return cur_index;
        }
        struct Selection* selection = selections->selections[cur_index];
        if (selection->depth == depth) {
            elog(LOG, "cur_index to return: %d", cur_index);
            return cur_index;
        } else if (is_disable_selection(selection)) {
            elog(LOG, "cur_index continue: %d", cur_index);
            continue;
        }
    }
}

void form_query_begin(char* query, int max_depth_index, struct Selections* selections) {
    // 10 - max int numbers count = 2 147 483 647
    int max_depth = selections->selections[max_depth_index]->depth;
    char* str = (char *)malloc(sizeof(char) * (sizeof("SELECT json_agg(\"sub/\")") + 10));
    sprintf(str, "SELECT json_agg(\"sub/%d\")", max_depth);
    strcat(query, str);
    memset(str, 0, sizeof(str));
    // sprintf(str, " FROM %s,", selections->selections[selections->count - 1]->name);
    sprintf(str, " FROM %s,", selections->selections[0]->name);
    strcat(query, str);
    free(str);
    // disable_selection(selections, 0);
}

void disable_selection(struct Selections* selections, int index) {
    selections->selections[index]->depth = 0;
}

int is_disable_selection(struct Selection* selection) {
    if (selection->depth == 0) {
        return 1;
    } else {
        return 0;
    }
}

int get_deepest(struct Selections* selections) {
    int max_depth_index = 0;
    size_t max_depth = 0;
    for (int i = 0; i < selections->count; i++) {
        if (selections->selections[i]->depth > max_depth) {
            max_depth = selections->selections[i]->depth;
            max_depth_index = i;
        }
    }
    return max_depth_index;
}
