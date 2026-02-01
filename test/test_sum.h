#ifndef TEST_SUM_H
#define TEST_SUM_H

#include <limits.h>
#include "test.h"
#include "sum.h"

void test_sum_positive_numbers()
{
    assert_equal_int(sum(2, 3), 5, "sum of 2 and 3");
    assert_equal_int(sum(10, 20), 30, "sum of 10 and 20");
    assert_equal_int(sum(100, 200), 300, "sum of 100 and 200");
}

void test_sum_negative_numbers()
{
    assert_equal_int(sum(-2, -3), -5, "sum of -2 and -3");
    assert_equal_int(sum(-10, -20), -30, "sum of -10 and -20");
    assert_equal_int(sum(-5, -5), -10, "sum of -5 and -5");
}

void test_sum_mixed_signs()
{
    assert_equal_int(sum(-5, 5), 0, "sum of -5 and 5");
    assert_equal_int(sum(10, -3), 7, "sum of 10 and -3");
    assert_equal_int(sum(-10, 3), -7, "sum of -10 and 3");
}

void test_sum_zero()
{
    assert_equal_int(sum(0, 0), 0, "sum of 0 and 0");
    assert_equal_int(sum(5, 0), 5, "sum of 5 and 0");
    assert_equal_int(sum(0, 5), 5, "sum of 0 and 5");
    assert_equal_int(sum(-5, 0), -5, "sum of -5 and 0");
    assert_equal_int(sum(0, -5), -5, "sum of 0 and -5");
}

void test_sum_large_numbers()
{
    assert_equal_int(sum(1000000, 2000000), 3000000, "sum of large numbers");
    assert_equal_int(sum(INT_MAX - 1, 1), INT_MAX, "sum near INT_MAX");
    assert_equal_int(sum(INT_MIN + 1, -1), INT_MIN, "sum near INT_MIN");
}

#endif
