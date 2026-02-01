#include "test.h"
#include <limits.h>

int main(void)
{
    begin_suite();

    begin_module("Test");
    end_module();

    int failed = end_suite();

    return failed ? 1 : 0;
}
