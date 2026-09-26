#include "../../s21_decimal.h"
#include "../../helpers/s21_utils.h"
#include "../test_runner.h"

START_TEST(div_result_zero_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    d2.bits[0] = 0x1;
    init_decimal(&result);
    init_decimal(&expected);

    int status = s21_div(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(div_zero_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    int status = s21_div(d1, d2, &result);
    ck_assert_int_eq(status, 3);
}
END_TEST

START_TEST(div_different_sign_test1)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 15;
    set_sign(&d1, 1);

    d2.bits[0] = 5;

    expected.bits[0] = 3;
    set_sign(&expected, 1);

    int status = s21_div(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(div_different_sign_test2)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 15;

    d2.bits[0] = 5;
    set_sign(&d2, 1);

    expected.bits[0] = 3;
    set_sign(&expected, 1);

    int status = s21_div(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(repeating_decimal_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 1;
    d2.bits[0] = 3;
    expected.bits[0] = 0x5555555;
    expected.bits[1] = 0x14B700CB;
    expected.bits[2] = 0xAC544CA;
    set_scale(&expected, 28);
    set_sign(&expected, 0);
    int status = s21_div(d1, d2, &result);

    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(div_overflow_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    d1.bits[2] = 0xFFFFFFFF;
    d2.bits[0] = 5;
    set_scale(&d2, 1);

    int status = s21_div(d1, d2, &result);

    ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(div_underflow_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    d1.bits[2] = 0xFFFFFFFF;

    set_sign(&d1, 1);

    d2.bits[0] = 5;
    set_scale(&d2, 1);

    int status = s21_div(d1, d2, &result);

    ck_assert_int_eq(status, 2);
}
END_TEST

START_TEST(div_different_scale_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 123456789;
    set_scale(&d1, 2);

    d2.bits[0] = 3;
    set_scale(&d2, 4);

    expected.bits[0] = 0xF5495EBC;

    int status = s21_div(d1, d2, &result);

    ck_assert_int_eq(status, 0);
    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(div_bank_rounding_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 15;
    set_scale(&d1, 28);
    d2.bits[0] = 2;

    expected.bits[0] = 8;
    expected.bits[1] = 0;
    expected.bits[2] = 0;
    set_scale(&expected, 28);
    set_sign(&expected, 0);

    int status = s21_div(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

Suite *s21_div_suite()
{
    Suite *s = suite_create("s21_div_suite");

    TCase *tc_core = tcase_create("Core_and_Zeros");
    tcase_add_test(tc_core, div_result_zero_test);
    tcase_add_test(tc_core, div_zero_test);
    suite_add_tcase(s, tc_core);

    TCase *tc_signs = tcase_create("Signs_Handling");
    tcase_add_test(tc_signs, div_different_sign_test1);
    tcase_add_test(tc_signs, div_different_sign_test2);
    suite_add_tcase(s, tc_signs);

    TCase *tc_overflow = tcase_create("Overflow_and_Underflow");
    tcase_add_test(tc_overflow, repeating_decimal_test);
    tcase_add_test(tc_overflow, div_overflow_test);
    tcase_add_test(tc_overflow, div_underflow_test);
    suite_add_tcase(s, tc_overflow);

    TCase *tc_complex = tcase_create("Scale_and_Rounding");
    tcase_add_test(tc_complex, div_different_scale_test);
    tcase_add_test(tc_complex, div_bank_rounding_test);
    suite_add_tcase(s, tc_complex);

    return s;
}