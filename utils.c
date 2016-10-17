#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "CuTest.h"

/*
 * Copies the given string into a new string. Note that the returned string
 * must be freed.
 */
char *new_str_from(const char *src) {
    const size_t src_size = strlen(src) + 1;
    char *dst = (char*) malloc(src_size);
    return strcpy(dst, src);
}

////////////////////////////////////////////////////////////////////////////////
// Unit tests
////////////////////////////////////////////////////////////////////////////////

void new_str_from_should_create_new_copy(CuTest *tc) {
    // Given
    const char *input = "hello";

    // When new_str_from is called
    char *actual = new_str_from(input);

    // Then
    const char *expected = "hello";
    CuAssertStrEquals(tc, expected, actual);
    free(actual);
}

CuSuite *get_utils_suite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, new_str_from_should_create_new_copy);
    return suite;
}
