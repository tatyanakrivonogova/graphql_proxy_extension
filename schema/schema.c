#include "schema.h"

#include "stdlib.h"

void free_arguments(Operation *operation) {
    if (operation == NULL) return;
    for (size_t i = 0; i < operation->argumentsNumber; ++i) {
        if (operation->arguments[i] != NULL) free(operation->arguments[i]);
    }
}