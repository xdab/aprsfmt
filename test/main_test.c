#include "test.h"
#include "test_sum.h"
#include <limits.h>

int main(void)
{
    begin_suite();

    begin_module("Sum");
    test_sum_positive_numbers();
    test_sum_negative_numbers();
    test_sum_mixed_signs();
    test_sum_zero();
    test_sum_large_numbers();
    end_module();

    int failed = end_suite();

    return failed ? 1 : 0;
}
