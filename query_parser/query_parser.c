#include "query_parser.h"

#include "schema/schema.h"
#include <stdio.h>

char* handle_operation_query(cJSON* definition) {
    cJSON* selection_set;
    char* response;
    struct Selections *selections = (struct Selection*)malloc(sizeof(struct Selection));
    memset(selections, 0, sizeof(selections));
    elog(LOG, "selection size: %d", sizeof(struct Selections));
    selection_set = cJSON_GetObjectItemCaseSensitive(definition, "selectionSet");
    int depth = 0;
    parse_selection_set(selections, selection_set, depth);
    log_stack(selections);
    response = make_sql_query(selections);
    elog(LOG, "sql query: %s", response);
    return response;
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
    int prev_depth_index = max_depth_index;
    form_query_begin(query, max_depth_index, selections);
    elog(LOG, "make query begin: %s", query);
    // form_layer_query(query, selections, max_depth_index);
    //todo: selections->selections[count - 1] - main table name
    int itteration = 1;
    while (max_depth_index > 0) {
        elog(LOG, "got deepest");
        prev_depth_index = max_depth_index;
        max_depth_index = get_deepest(selections);
        if (max_depth_index == count) {
            return query;
        }
        form_layer_query(query, selections, max_depth_index, prev_depth_index, itteration);
        itteration++;
    }
    elog(LOG, "query: \n%s", query);
    return query;
}

void form_layer_query(char* query, struct Selections* selections, int layer_index, int prev_depth_index, int itteration) {
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
    add_from(query, table_name);
    add_where(query, selections, layer_index, prev_depth_index, itteration);
    add_final_symbols(query, itteration, depth);
    elog(LOG, "query: %s", query);
    free(tmp);
}

void add_where(char* query, struct Selections* selections, int layer_index, int prev_layer_index, int itteration) {
    bool is_complex = false;
    if (itteration > 1 || selections->selections[layer_index]->argName != NULL) {
        strcat(query, "\n\t\tWHERE ");
    } else {
        return;
    }
    if (selections->selections[layer_index]->argName != NULL) {
        char* tmp = (char *)malloc(sizeof(char) * (NAME_LENGTH * 2));
        sprintf(tmp, "%s.%s = '%s'", selections->selections[layer_index]->name, selections->selections[layer_index]->argName,
            selections->selections[layer_index]->argValue);
        strcat(query, tmp);
        free(tmp);
        is_complex = true;
    }
    if (itteration > 1) {
        if (is_complex) {
            strcat(query, " AND\n\t\t");
        }
        char* tmp = (char *)malloc(sizeof(char) * (NAME_LENGTH * 2));
        char* key = selections->selections[prev_layer_index]->name;
        sprintf(tmp, "%s.%s = %s.pk_%s", selections->selections[layer_index]->name, key, key, key);
        strcat(query, tmp);
        free(tmp);
    }
}

void add_from(char* query, char* table_name) {
    char* tmp = (char *)malloc(sizeof(char) * (NAME_LENGTH * 2));
    sprintf(tmp, "\n\t\tFROM %s", table_name);
    strcat(query, tmp);
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

void add_final_symbols(char* query, int itteration, int depth) {
    char* tmp = (char *)malloc(sizeof(char) * (NAME_LENGTH * 3));
    sprintf(tmp, "\n\t) AS \"sub/%d\"", itteration);
    strcat(query, tmp);
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
