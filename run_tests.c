#include <stdio.h>
#include "CuTest.h"

CuSuite *get_utils_suite();

int main(int argc, char *argv[]) {
    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, get_utils_suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s\n", output->buffer);

    return suite->failCount;
}
