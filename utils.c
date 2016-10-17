#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*
 * Copies the given string into a new string. Note that the returned string
 * must be freed.
 */
char *new_str_from(const char *src) {
    const size_t src_size = strlen(src) + 1;
    char *dst = (char*) malloc(src_size);
    return strcpy(dst, src);
}
