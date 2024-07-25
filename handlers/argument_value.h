#include <stdbool.h>

#define NAME_LENGTH 63
#define TYPE_LENGTH 63
#define VALUE_LENGTH 63
#define MAX_ARGUMENTS_NUMBER 20

typedef struct {
    char argName[NAME_LENGTH];
    char argType[TYPE_LENGTH];
    char value[VALUE_LENGTH];
    bool isNull;
} ArgValue;

typedef struct {
    size_t argNumber;
    ArgValue *argValues[MAX_ARGUMENTS_NUMBER];
} ArgValues;