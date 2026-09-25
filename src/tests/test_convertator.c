#include "test_main.h"
#include "../s21_decimal.h"

/* ========================================================================== */
/*                         s21_from_decimal_to_int                            */
/* ========================================================================== */

START_TEST(test_from_decimal_to_int_zero) {
    s21_decimal src = {{0, 0, 0, 0}};
    int dst = -1;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_positive) {
    s21_decimal src = {{42, 0, 0, 0}};
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, 42);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative) {
    s21_decimal src = {{42, 0, 0, (int)0x80000000}};  // -42
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, -42);
}
END_TEST

START_TEST(test_from_decimal_to_int_max) {
    s21_decimal src = {{2147483647, 0, 0, 0}};  // INT_MAX
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, 2147483647);
}
END_TEST

START_TEST(test_from_decimal_to_int_min) {
    s21_decimal src = {{(int)2147483648U, 0, 0, (int)0x80000000}};  // INT_MIN
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, -2147483648);
}
END_TEST

START_TEST(test_from_decimal_to_int_overflow_positive) {
    s21_decimal src = {{(int)2147483648U, 0, 0, 0}};  // INT_MAX + 1
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_overflow_negative) {
    s21_decimal src = {{(int)2147483649U, 0, 0, (int)0x80000000}};
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_overflow_high_bits) {
    s21_decimal src = {{0, 1, 0, 0}};  // bits[1] != 0
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_truncate_positive) {
    // 9.9 => scale=1, bits[0]=99 => truncate => 9
    s21_decimal src = {{99, 0, 0, 1 << 16}};
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, 9);
}
END_TEST

START_TEST(test_from_decimal_to_int_truncate_negative) {
    // -9.9 => truncate => -9
    s21_decimal src = {{99, 0, 0, (int)0x80010000}};  // sign=1, scale=1
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, -9);
}
END_TEST

START_TEST(test_from_decimal_to_int_scale_2) {
    // 123.45 => scale=2, bits[0]=12345 => truncate => 123
    s21_decimal src = {{12345, 0, 0, 2 << 16}};
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, 123);
}
END_TEST

START_TEST(test_from_decimal_to_int_dst_null) {
    s21_decimal src = {{1, 0, 0, 0}};
    ck_assert_int_eq(s21_from_decimal_to_int(src, NULL), 1);
}
END_TEST

START_TEST(test_from_decimal_to_int_negative_zero) {
    // -0.5 => truncate => 0
    s21_decimal src = {{5, 0, 0, (int)0x80010000}};  // -0.5
    int dst = -1;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 0);
    ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(test_from_decimal_to_int_invalid_scale) {
    // scale = 29 (невалидный) => return 1
    s21_decimal src = {{1, 0, 0, 29 << 16}};
    int dst = 0;
    ck_assert_int_eq(s21_from_decimal_to_int(src, &dst), 1);
}
END_TEST

/* ========================================================================== */
/*                         s21_from_int_to_decimal                            */
/* ========================================================================== */

START_TEST(test_from_int_to_decimal_zero) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_int_to_decimal(0, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 0);
    ck_assert_int_eq(dst.bits[1], 0);
    ck_assert_int_eq(dst.bits[2], 0);
    ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_positive) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_int_to_decimal(42, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 42);
    ck_assert_int_eq(dst.bits[1], 0);
    ck_assert_int_eq(dst.bits[2], 0);
    ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_negative) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_int_to_decimal(-42, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 42);
    ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_from_int_to_decimal_int_max) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_int_to_decimal(2147483647, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 2147483647);
    ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_from_int_to_decimal_int_min) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_int_to_decimal(-2147483648, &dst), 0);
    ck_assert_uint_eq((unsigned int)dst.bits[0], 2147483648U);
    ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_from_int_to_decimal_dst_null) {
    ck_assert_int_eq(s21_from_int_to_decimal(1, NULL), 1);
}
END_TEST

/* ========================================================================== */
/*                           Roundtrip тесты                                  */
/* ========================================================================== */

START_TEST(test_roundtrip_positive) {
    s21_decimal d;
    int result = 0;
    s21_from_int_to_decimal(12345, &d);
    s21_from_decimal_to_int(d, &result);
    ck_assert_int_eq(result, 12345);
}
END_TEST

START_TEST(test_roundtrip_negative) {
    s21_decimal d;
    int result = 0;
    s21_from_int_to_decimal(-12345, &d);
    s21_from_decimal_to_int(d, &result);
    ck_assert_int_eq(result, -12345);
}
END_TEST

START_TEST(test_roundtrip_zero) {
    s21_decimal d;
    int result = -1;
    s21_from_int_to_decimal(0, &d);
    s21_from_decimal_to_int(d, &result);
    ck_assert_int_eq(result, 0);
}
END_TEST

/* ========================================================================== */
/*                              s21_negate                                    */
/* ========================================================================== */

START_TEST(test_negate_positive_to_negative) {
    s21_decimal src = {{42, 0, 0, 0}};  // +42
    s21_decimal dst;
    ck_assert_int_eq(s21_negate(src, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 42);
    ck_assert_int_eq(dst.bits[3], (int)0x80000000);  // sign=1
}
END_TEST

START_TEST(test_negate_negative_to_positive) {
    s21_decimal src = {{42, 0, 0, (int)0x80000000}};  // -42
    s21_decimal dst;
    ck_assert_int_eq(s21_negate(src, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 42);
    ck_assert_int_eq(dst.bits[3], 0);  // sign=0
}
END_TEST

START_TEST(test_negate_zero) {
    s21_decimal src = {{0, 0, 0, 0}};  // +0
    s21_decimal dst;
    ck_assert_int_eq(s21_negate(src, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 0);
    ck_assert_int_eq(dst.bits[1], 0);
    ck_assert_int_eq(dst.bits[2], 0);
}
END_TEST

START_TEST(test_negate_preserves_scale) {
    // 1.23 => scale=2, bits[0]=123
    s21_decimal src = {{123, 0, 0, 2 << 16}};
    s21_decimal dst;
    ck_assert_int_eq(s21_negate(src, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 123);
    // scale должен остаться 2, sign стать 1
    ck_assert_int_eq(dst.bits[3], (int)0x80020000);
}
END_TEST

START_TEST(test_negate_preserves_all_bits) {
    s21_decimal src = {{(int)0xDEADBEEF, 0x12345678, 0x0ABCDEF0, 0}};
    s21_decimal dst;
    ck_assert_int_eq(s21_negate(src, &dst), 0);
    ck_assert_int_eq(dst.bits[0], (int)0xDEADBEEF);
    ck_assert_int_eq(dst.bits[1], 0x12345678);
    ck_assert_int_eq(dst.bits[2], 0x0ABCDEF0);
    ck_assert_int_eq(dst.bits[3], (int)0x80000000);  // только знак изменился
}
END_TEST

START_TEST(test_negate_does_not_modify_src) {
    s21_decimal src = {{99, 0, 0, 0}};
    s21_decimal dst;
    s21_negate(src, &dst);
    // src передан по значению — оригинал не должен измениться
    ck_assert_int_eq(src.bits[0], 99);
    ck_assert_int_eq(src.bits[3], 0);
}
END_TEST

START_TEST(test_negate_dst_null) {
    s21_decimal src = {{1, 0, 0, 0}};
    ck_assert_int_eq(s21_negate(src, NULL), 1);
}
END_TEST

START_TEST(test_negate_double) {
    // двойное отрицание должно вернуть исходное значение
    s21_decimal src = {{777, 0, 0, 3 << 16}};  // 0.777
    s21_decimal tmp, dst;
    s21_negate(src, &tmp);
    s21_negate(tmp, &dst);
    ck_assert_int_eq(dst.bits[0], src.bits[0]);
    ck_assert_int_eq(dst.bits[3], src.bits[3]);
}
END_TEST

/* ========================================================================== */
/*                         s21_from_decimal_to_float                          */
/* ========================================================================== */

START_TEST(test_from_decimal_to_float_zero) {
    s21_decimal src = {{0, 0, 0, 0}};
    float dst = -1.0f;
    ck_assert_int_eq(s21_from_decimal_to_float(src, &dst), 0);
    ck_assert_float_eq(dst, 0.0f);
}
END_TEST

START_TEST(test_from_decimal_to_float_negative_zero) {
    s21_decimal src = {{0, 0, 0, (int)0x80000000}};
    float dst = 1.0f;
    ck_assert_int_eq(s21_from_decimal_to_float(src, &dst), 0);
    ck_assert_float_eq(dst, 0.0f);
    ck_assert_int_ne(signbit(dst), 0);
}
END_TEST

START_TEST(test_from_decimal_to_float_positive) {
    s21_decimal src = {{123456, 0, 0, 3 << 16}};  // 123.456
    float dst = 0.0f;
    ck_assert_int_eq(s21_from_decimal_to_float(src, &dst), 0);
    ck_assert_float_eq_tol(dst, 123.456f, 1e-4f);
}
END_TEST

START_TEST(test_from_decimal_to_float_negative) {
    s21_decimal src = {{123456, 0, 0, (int)0x80030000}};  // -123.456
    float dst = 0.0f;
    ck_assert_int_eq(s21_from_decimal_to_float(src, &dst), 0);
    ck_assert_float_eq_tol(dst, -123.456f, 1e-4f);
}
END_TEST

START_TEST(test_from_decimal_to_float_null) {
    s21_decimal src = {{1, 0, 0, 0}};
    ck_assert_int_eq(s21_from_decimal_to_float(src, NULL), 1);
}
END_TEST

START_TEST(test_from_decimal_to_float_invalid_scale) {
    s21_decimal src = {{1, 0, 0, 29 << 16}};
    float dst = 0.0f;
    ck_assert_int_eq(s21_from_decimal_to_float(src, &dst), 1);
}
END_TEST

/* ========================================================================== */
/*                         s21_from_float_to_decimal                          */
/* ========================================================================== */

START_TEST(test_from_float_to_decimal_zero) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(0.0f, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 0);
    ck_assert_int_eq(dst.bits[1], 0);
    ck_assert_int_eq(dst.bits[2], 0);
    ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_from_float_to_decimal_negative_zero) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(-0.0f, &dst), 0);
    ck_assert_int_eq(dst.bits[0], 0);
    ck_assert_int_eq(dst.bits[1], 0);
    ck_assert_int_eq(dst.bits[2], 0);
    ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_from_float_to_decimal_positive) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(123.456f, &dst), 0);
    float back = 0.0f;
    s21_from_decimal_to_float(dst, &back);
    ck_assert_float_eq_tol(back, 123.456f, 1e-4f);
}
END_TEST

START_TEST(test_from_float_to_decimal_negative) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(-12.34567f, &dst), 0);
    float back = 0.0f;
    s21_from_decimal_to_float(dst, &back);
    ck_assert_float_eq_tol(back, -12.34567f, 1e-4f);
}
END_TEST

START_TEST(test_from_float_to_decimal_too_small) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(1e-29f, &dst), 1);
    ck_assert_int_eq(dst.bits[0], 0);
    ck_assert_int_eq(dst.bits[1], 0);
    ck_assert_int_eq(dst.bits[2], 0);
    ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_from_float_to_decimal_too_large) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(1e30f, &dst), 1);
}
END_TEST

START_TEST(test_from_float_to_decimal_nan_inf) {
    s21_decimal dst;
    ck_assert_int_eq(s21_from_float_to_decimal(NAN, &dst), 1);
    ck_assert_int_eq(s21_from_float_to_decimal(INFINITY, &dst), 1);
    ck_assert_int_eq(s21_from_float_to_decimal(-INFINITY, &dst), 1);
}
END_TEST

START_TEST(test_from_float_to_decimal_null) {
    ck_assert_int_eq(s21_from_float_to_decimal(1.0f, NULL), 1);
}
END_TEST

/* ========================================================================== */
/*                               Suite Setup                                  */
/* ========================================================================== */

Suite *suite_convertator(void) {
    Suite *s = suite_create("s21_convertator");

    TCase *tc_to_int = tcase_create("from_decimal_to_int");
    tcase_add_test(tc_to_int, test_from_decimal_to_int_zero);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_positive);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_negative);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_max);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_min);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_overflow_positive);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_overflow_negative);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_overflow_high_bits);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_truncate_positive);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_truncate_negative);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_scale_2);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_dst_null);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_negative_zero);
    tcase_add_test(tc_to_int, test_from_decimal_to_int_invalid_scale);
    suite_add_tcase(s, tc_to_int);

    TCase *tc_from_int = tcase_create("from_int_to_decimal");
    tcase_add_test(tc_from_int, test_from_int_to_decimal_zero);
    tcase_add_test(tc_from_int, test_from_int_to_decimal_positive);
    tcase_add_test(tc_from_int, test_from_int_to_decimal_negative);
    tcase_add_test(tc_from_int, test_from_int_to_decimal_int_max);
    tcase_add_test(tc_from_int, test_from_int_to_decimal_int_min);
    tcase_add_test(tc_from_int, test_from_int_to_decimal_dst_null);
    suite_add_tcase(s, tc_from_int);

    TCase *tc_to_float = tcase_create("from_decimal_to_float");
    tcase_add_test(tc_to_float, test_from_decimal_to_float_zero);
    tcase_add_test(tc_to_float, test_from_decimal_to_float_negative_zero);
    tcase_add_test(tc_to_float, test_from_decimal_to_float_positive);
    tcase_add_test(tc_to_float, test_from_decimal_to_float_negative);
    tcase_add_test(tc_to_float, test_from_decimal_to_float_null);
    tcase_add_test(tc_to_float, test_from_decimal_to_float_invalid_scale);
    suite_add_tcase(s, tc_to_float);

    TCase *tc_from_float = tcase_create("from_float_to_decimal");
    tcase_add_test(tc_from_float, test_from_float_to_decimal_zero);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_negative_zero);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_positive);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_negative);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_too_small);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_too_large);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_nan_inf);
    tcase_add_test(tc_from_float, test_from_float_to_decimal_null);
    suite_add_tcase(s, tc_from_float);

    TCase *tc_roundtrip = tcase_create("roundtrip");
    tcase_add_test(tc_roundtrip, test_roundtrip_positive);
    tcase_add_test(tc_roundtrip, test_roundtrip_negative);
    tcase_add_test(tc_roundtrip, test_roundtrip_zero);
    suite_add_tcase(s, tc_roundtrip);

    TCase *tc_negate = tcase_create("negate");
    tcase_add_test(tc_negate, test_negate_positive_to_negative);
    tcase_add_test(tc_negate, test_negate_negative_to_positive);
    tcase_add_test(tc_negate, test_negate_zero);
    tcase_add_test(tc_negate, test_negate_preserves_scale);
    tcase_add_test(tc_negate, test_negate_preserves_all_bits);
    tcase_add_test(tc_negate, test_negate_does_not_modify_src);
    tcase_add_test(tc_negate, test_negate_dst_null);
    tcase_add_test(tc_negate, test_negate_double);
    suite_add_tcase(s, tc_negate);

    return s;
}

