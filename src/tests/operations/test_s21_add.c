#include "../../s21_decimal.h"
#include "../../helpers/s21_utils.h"
#include "../test_runner.h"

START_TEST(add_zero_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}
END_TEST

START_TEST(different_sign_test1)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    set_sign(&d1, 1);
    d1.bit[0] = 0xF;
    d2.bit[0] = 0xFFFFFFFF;
    expected.bit[0] = 0xFFFFFFF0;
    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}
END_TEST

START_TEST(different_sign_test2)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    set_sign(&d2, 1);
    d1.bit[0] = 0xFFFFFFFF;
    d2.bit[0] = 0xF;
    expected.bit[0] = 0xFFFFFFF0;
    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}

END_TEST

START_TEST(overflow_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bit[0] = 0xFFFFFFFF;
    d2.bit[0] = 0xFFFFFFFF;
    expected.bit[0] = 0xFFFFFFFE;
    expected.bit[1] = 0x00000001;
    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}
END_TEST

START_TEST(negetive_result_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);
    set_sign(&d2, 1);
    d1.bit[0] = 0x0;
    d2.bit[0] = 0xF;
    expected.bit[0] = 0xF;
    set_sign(&expected, 1);
    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}
END_TEST

START_TEST(domino_overflow_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    d1.bit[0] = 0xFFFFFFFF;
    d1.bit[1] = 0xFFFFFFFF;
    d1.bit[2] = 0xFFFFFFFF;

    d2.bit[0] = 0x1;

    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(underflow_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    d1.bit[0] = 0xFFFFFFFF;
    d1.bit[1] = 0xFFFFFFFF;
    d1.bit[2] = 0xFFFFFFFF;
    set_sign(&d1, 1);

    d2.bit[0] = 0x1;
    set_sign(&d2, 1);

    int status = s21_add(d1, d2, &result);

    ck_assert_int_eq(status, 2);
}
END_TEST

START_TEST(different_scale_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);
    d1.bit[0] = 0xF2F;
    d2.bit[0] = 0xF;
    set_scale(&d1, 2);
    set_scale(&d2, 1);
    expected.bit[0] = 0xFC5;
    set_scale(&expected, 2);
    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}
END_TEST

START_TEST(bank_rounding_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);
    d1.bit[0] = 0xFFFFFFFD;
    d1.bit[1] = 0xFFFFFFFF;
    d1.bit[2] = 0xFFFFFFFF;
    set_scale(&d1, 1);
    d2.bit[0] = 5;
    set_scale(&d2, 2);

    expected.bit[0] = 0xFFFFFFFD;
    expected.bit[1] = 0xFFFFFFFF;
    expected.bit[2] = 0xFFFFFFFF;
    set_scale(&expected, 1);

    int status = s21_add(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bit[i], expected.bit[i]);
}
END_TEST

Suite *s21_add_suite()
{
    Suite *s = suite_create("s21_add_suite");

    TCase *tc_core = tcase_create("Core_and_Zeros");
    tcase_add_test(tc_core, add_zero_test);
    suite_add_tcase(s, tc_core);

    TCase *tc_signs = tcase_create("Signs_Handling");
    tcase_add_test(tc_signs, different_sign_test1);
    tcase_add_test(tc_signs, different_sign_test2);
    tcase_add_test(tc_signs, negetive_result_test);
    suite_add_tcase(s, tc_signs);

    TCase *tc_overflow = tcase_create("Overflow_and_Underflow");
    tcase_add_test(tc_overflow, overflow_test);
    tcase_add_test(tc_overflow, domino_overflow_test);
    tcase_add_test(tc_overflow, underflow_test);
    suite_add_tcase(s, tc_overflow);

    TCase *tc_complex = tcase_create("Scale_and_Rounding");
    tcase_add_test(tc_complex, different_scale_test);
    tcase_add_test(tc_complex, bank_rounding_test);
    suite_add_tcase(s, tc_complex);

    return s;
}