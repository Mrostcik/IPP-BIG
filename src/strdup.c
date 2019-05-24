#include <stdio.h>
#include <stdlib.h>

#include "strdup.h"

char *strDuplicate(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}
