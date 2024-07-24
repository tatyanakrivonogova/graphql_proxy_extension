#include "schema.h"

#include "stdlib.h"

void free_arguments(Mutation *mutation) {
    if (mutation == NULL) return;
    for (size_t i = 0; i < mutation->argumentsNumber; ++i) {
        if (mutation->arguments[i] != NULL) free(mutation->arguments[i]);
    }
}