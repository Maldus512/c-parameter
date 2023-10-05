#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "parameter.h"

#define DEFAULT_AL 1


/* These functions will be used to initialize
   and clean resources up after each test run */
int setup(void **state) {
    (void)state;
    return 0;
}

int teardown(void **state) {
    (void)state;
    return 0;
}


void test_basic(void **state) {
    (void)state;
    uint32_t           value = 5;
    parameter_handle_t par1  = PARAMETER(&value, 0, 10, 2, 0, DEFAULT_AL);
    assert_int_equal(10, par1.max.u32);

    parameter_operator(&par1, 1);
    assert_int_equal(6, value);
    parameter_operator(&par1, 1);
    parameter_operator(&par1, 1);
    parameter_operator(&par1, 1);
    parameter_operator(&par1, 1);
    assert_int_equal(10, value);
    parameter_operator(&par1, 1);
    assert_int_equal(0, value);

    uint32_t           max  = 12;
    parameter_handle_t par2 = PARAMETER_DLIMITS(&value, NULL, &max, 1, 0, 0, 0, DEFAULT_AL);
    assert_int_equal(0, par2.max.u32);
    assert_int_equal(&max, par2.pmax);
}


void test_corrupted_value(void **state) {
    (void)state;
    int           value1 = 5;
    unsigned char value2 = 10;
    float         value3 = .5;
    unsigned long value4 = 500000;

    parameter_handle_t ps[] = {
        PARAMETER(&value1, 0, 10, 1, 0, DEFAULT_AL),
        PARAMETER(&value2, 0, 100, 2, 0, DEFAULT_AL),
        PARAMETER(&value3, 0, 10, 3, 0, DEFAULT_AL),
        PARAMETER(&value4, 0, 1000000, 4, 0, DEFAULT_AL),
    };

    assert_int_equal(0, parameter_check_ranges(ps, 4));
    value4 *= 4;
    assert_int_equal(1, parameter_check_ranges(ps, 4));
    assert_int_equal(0, parameter_check_ranges(ps, 4));
    value1 = 1000;
    value2 = 101;
    value3 = 10.5;
    assert_int_equal(3, parameter_check_ranges(ps, 4));

    assert_int_equal(1, value1);
    assert_int_equal(2, value2);
    assert_int_equal(3, value3);
    assert_int_equal(4, value4);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_basic),
        cmocka_unit_test(test_corrupted_value),
    };

    /* If setup and teardown functions are not
       needed, then NULL may be passed instead */

    int count_fail_tests = cmocka_run_group_tests(tests, setup, teardown);

    return count_fail_tests;
}
