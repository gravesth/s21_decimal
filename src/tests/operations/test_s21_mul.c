#include "../../s21_decimal.h"
#include "../../helpers/s21_utils.h"
#include "../test_runner.h"

START_TEST(mul_zero_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
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
    d1.bits[0] = 0xF;
    d2.bits[0] = 0xF;
    expected.bits[0] = 0xE1;
    set_sign(&expected, 1);
    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
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
    d1.bits[0] = 0xF;
    d2.bits[0] = 0xF;
    expected.bits[0] = 0xE1;
    set_sign(&expected, 1);
    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(overflow_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 0x186A0;
    d2.bits[0] = 0x186A0;
    expected.bits[0] = 0x540BE400;
    expected.bits[1] = 0x00000002;
    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(basic_test)
{
    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);
    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    d1.bits[2] = 0x00000000;
    d2.bits[0] = 5;
    expected.bits[0] = 0xFFFFFFFB;
    expected.bits[1] = 0xFFFFFFFF;
    expected.bits[2] = 0x00000004;
    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

START_TEST(domino_overflow_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    d1.bits[2] = 0xFFFFFFFF;

    d2.bits[0] = 0x2;

    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(underflow_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    d1.bits[2] = 0xFFFFFFFF;

    d2.bits[0] = 0x2;
    set_sign(&d2, 1);

    int status = s21_mul(d1, d2, &result);

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
    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    set_scale(&d1, 3);

    d2.bits[0] = 3;
    set_scale(&d2, 2);

    expected.bits[0] = 0xFFFFFFFD;
    expected.bits[1] = 0xFFFFFFFF;
    expected.bits[2] = 0x00000002;
    set_scale(&expected, 5);
    set_sign(&expected, 0);

    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST


START_TEST(bank_rounding_test)
{

    s21_decimal d1, d2, result, expected;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);
    init_decimal(&expected);

    d1.bits[0] = 0xFFFFFFFF;
    d1.bits[1] = 0xFFFFFFFF;
    d1.bits[2] = 0xFFFFFFFF;

    d2.bits[0] = 33;
    set_scale(&d2, 2);

    expected.bits[0] = 0x47AE147B;
    expected.bits[1] = 0xAE147AE1;
    expected.bits[2] = 0x547AE147;
    set_scale(&expected, 0);
    set_sign(&expected, 0);

    int status = s21_mul(d1, d2, &result);
    ck_assert_int_eq(status, 0);

    for (int i = 0; i < 4; i++)
        ck_assert_uint_eq(result.bits[i], expected.bits[i]);
}
END_TEST

Suite *s21_mul_suite()
{
    Suite *s = suite_create("s21_mul_suite");

    TCase *tc_core = tcase_create("Core_and_Zeros");
    tcase_add_test(tc_core, mul_zero_test);
    tcase_add_test(tc_core, basic_test);
    suite_add_tcase(s, tc_core);

    TCase *tc_signs = tcase_create("Signs_Handling");
    tcase_add_test(tc_signs, different_sign_test1);
    tcase_add_test(tc_signs, different_sign_test2);
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