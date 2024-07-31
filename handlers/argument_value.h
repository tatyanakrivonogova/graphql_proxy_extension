#ifndef ARGUMENT_VALUE_H_
#define ARGUMENT_VALUE_H_

#include <stdbool.h>
#include <stddef.h>

#define NAME_LENGTH 63
#define TYPE_LENGTH 63
#define VALUE_LENGTH 63
#define MAX_ARGUMENTS_NUMBER 20
#define MAX_SELECTIONS 20

typedef struct ArgValue{
    char argName[NAME_LENGTH];
    char argType[TYPE_LENGTH];
    char value[VALUE_LENGTH];
    bool isNull;
} ArgValue;

typedef struct ArgValues{
    size_t argNumber;
    struct ArgValue *argValues[MAX_ARGUMENTS_NUMBER];
} ArgValues;

typedef struct Selection{
    char name[NAME_LENGTH];
    char argName[TYPE_LENGTH];
    char argValue[VALUE_LENGTH];
    int argCount;
    int depth;
    bool is_selection_set;
} Selection;

typedef struct Selections{
    size_t count;
    struct Selection *selections[MAX_SELECTIONS];
} Selections;

#endif