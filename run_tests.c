#include <stdio.h>
#include "CuTest.h"

CuSuite *get_utils_suite();
CuSuite *get_room_suite();

int main(int argc, char *argv[]) {
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, get_utils_suite());
    CuSuiteAddSuite(suite, get_room_suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s\n", output->buffer);

    return suite->failCount;
}
