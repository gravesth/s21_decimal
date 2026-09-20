#include "test_main.h"

/* ========================================================================== */
/*                                s21_is_equal                                */
/* ========================================================================== */

START_TEST(test_is_equal_basic) {
    s21_decimal a = {{12345, 0, 0, 0}};
    s21_decimal b = {{12345, 0, 0, 0}};
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_negatives) {
    s21_decimal a = {{12345, 0, 0, (int)0x80000000}};
    s21_decimal b = {{12345, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_zeroes) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};  // -0
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_zeroes_scaled) {
    s21_decimal a = {{0, 0, 0, 2 << 16}};          // 0.00
    s21_decimal b = {{0, 0, 0, (int)0x80050000}};  // -0.00000
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_different_scales) {
    s21_decimal a = {{15, 0, 0, 1 << 16}};    // 1.5
    s21_decimal b = {{1500, 0, 0, 3 << 16}};  // 1.500
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_equal_different_signs) {
    s21_decimal a = {{100, 0, 0, 0}};
    s21_decimal b = {{100, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_equal_different_values) {
    s21_decimal a = {{100, 0, 0, 0}};
    s21_decimal b = {{101, 0, 0, 0}};
    ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_equal_large_numbers) {
    s21_decimal a = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    s21_decimal b = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

/* ========================================================================== */
/*                              s21_is_not_equal                              */
/* ========================================================================== */

START_TEST(test_is_not_equal_basic) {
    s21_decimal a = {{50, 0, 0, 0}};
    s21_decimal b = {{60, 0, 0, 0}};
    ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_not_equal_same) {
    s21_decimal a = {{50, 0, 0, 0}};
    s21_decimal b = {{50, 0, 0, 0}};
    ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_not_equal_zeroes) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(test_is_not_equal_diff_signs) {
    s21_decimal a = {{50, 0, 0, 0}};
    s21_decimal b = {{50, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(test_is_not_equal_diff_scales_same_val) {
    s21_decimal a = {{120, 0, 0, 1 << 16}};   // 12.0
    s21_decimal b = {{1200, 0, 0, 2 << 16}};  // 12.00
    ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

/* ========================================================================== */
/*                                s21_is_less                                 */
/* ========================================================================== */

START_TEST(test_is_less_positive) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{10, 0, 0, 0}};
    ck_assert_int_eq(s21_is_less(a, b), 1);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_equal_values) {
    s21_decimal a = {{10, 0, 0, 0}};
    s21_decimal b = {{10, 0, 0, 0}};
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_is_less_negatives) {
    s21_decimal a = {{10, 0, 0, (int)0x80000000}};  // -10
    s21_decimal b = {{5, 0, 0, (int)0x80000000}};   // -5
    ck_assert_int_eq(s21_is_less(a, b), 1);         // -10 < -5 is TRUE
    ck_assert_int_eq(s21_is_less(b, a), 0);         // -5 < -10 is FALSE
}
END_TEST

START_TEST(test_is_less_mixed_signs) {
    s21_decimal a = {{5, 0, 0, (int)0x80000000}};  // -5
    s21_decimal b = {{5, 0, 0, 0}};                // +5
    ck_assert_int_eq(s21_is_less(a, b), 1);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_zeroes) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_less(a, b), 0);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_zero_and_numbers) {
    s21_decimal zero = {{0, 0, 0, 0}};
    s21_decimal pos = {{1, 0, 0, 0}};
    s21_decimal neg = {{1, 0, 0, (int)0x80000000}};

    ck_assert_int_eq(s21_is_less(zero, pos), 1);   // 0 < 1 is TRUE
    ck_assert_int_eq(s21_is_less(pos, zero), 0);   // 1 < 0 is FALSE
    ck_assert_int_eq(s21_is_less(neg, zero), 1);   // -1 < 0 is TRUE
    ck_assert_int_eq(s21_is_less(zero, neg), 0);   // 0 < -1 is FALSE
}
END_TEST

START_TEST(test_is_less_different_scales) {
    s21_decimal a = {{5, 0, 0, 2 << 16}};   // 0.05
    s21_decimal b = {{1, 0, 0, 1 << 16}};   // 0.10
    ck_assert_int_eq(s21_is_less(a, b), 1);  // 0.05 < 0.1
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_different_scales_neg) {
    s21_decimal a = {{1, 0, 0, (int)0x80010000}};  // -0.1
    s21_decimal b = {{5, 0, 0, (int)0x80020000}};  // -0.05
    ck_assert_int_eq(s21_is_less(a, b), 1);        // -0.1 < -0.05 is TRUE
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

START_TEST(test_is_less_high_bits) {
    s21_decimal a = {{0xFFFFFFFF, 1, 0, 0}};
    s21_decimal b = {{0xFFFFFFFF, 2, 0, 0}};
    ck_assert_int_eq(s21_is_less(a, b), 1);
    ck_assert_int_eq(s21_is_less(b, a), 0);
}
END_TEST

/* ========================================================================== */
/*                            s21_is_less_or_equal                            */
/* ========================================================================== */

START_TEST(test_is_less_or_equal_basic) {
    s21_decimal a = {{5, 0, 0, 0}};
    s21_decimal b = {{10, 0, 0, 0}};
    s21_decimal c = {{5, 0, 0, 0}};

    ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
    ck_assert_int_eq(s21_is_less_or_equal(a, c), 1);
    ck_assert_int_eq(s21_is_less_or_equal(b, a), 0);
}
END_TEST

START_TEST(test_is_less_or_equal_zeroes) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
    ck_assert_int_eq(s21_is_less_or_equal(b, a), 1);
}
END_TEST

/* ========================================================================== */
/*                               s21_is_greater                               */
/* ========================================================================== */

START_TEST(test_is_greater_positive) {
    s21_decimal a = {{10, 0, 0, 0}};
    s21_decimal b = {{5, 0, 0, 0}};
    ck_assert_int_eq(s21_is_greater(a, b), 1);
    ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

START_TEST(test_is_greater_equal) {
    s21_decimal a = {{10, 0, 0, 0}};
    s21_decimal b = {{10, 0, 0, 0}};
    ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_is_greater_negatives) {
    s21_decimal a = {{5, 0, 0, (int)0x80000000}};   // -5
    s21_decimal b = {{10, 0, 0, (int)0x80000000}};  // -10
    ck_assert_int_eq(s21_is_greater(a, b), 1);       // -5 > -10 is TRUE
    ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

START_TEST(test_is_greater_zeroes) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_greater(a, b), 0);
    ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

START_TEST(test_is_greater_scales) {
    s21_decimal a = {{1, 0, 0, 1 << 16}};   // 0.1
    s21_decimal b = {{5, 0, 0, 2 << 16}};   // 0.05
    ck_assert_int_eq(s21_is_greater(a, b), 1);
    ck_assert_int_eq(s21_is_greater(b, a), 0);
}
END_TEST

/* ========================================================================== */
/*                          s21_is_greater_or_equal                           */
/* ========================================================================== */

START_TEST(test_is_greater_or_equal_basic) {
    s21_decimal a = {{10, 0, 0, 0}};
    s21_decimal b = {{5, 0, 0, 0}};
    s21_decimal c = {{10, 0, 0, 0}};

    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
    ck_assert_int_eq(s21_is_greater_or_equal(a, c), 1);
    ck_assert_int_eq(s21_is_greater_or_equal(b, a), 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_zeroes) {
    s21_decimal a = {{0, 0, 0, 0}};
    s21_decimal b = {{0, 0, 0, (int)0x80000000}};
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
    ck_assert_int_eq(s21_is_greater_or_equal(b, a), 1);
}
END_TEST

/* ========================================================================== */
/*                                Suite Setup                                 */
/* ========================================================================== */

Suite *suite_comparison(void) {
    Suite *s = suite_create("s21_comparison");

    TCase *tc_equal = tcase_create("is_equal");
    tcase_add_test(tc_equal, test_is_equal_basic);
    tcase_add_test(tc_equal, test_is_equal_negatives);
    tcase_add_test(tc_equal, test_is_equal_zeroes);
    tcase_add_test(tc_equal, test_is_equal_zeroes_scaled);
    tcase_add_test(tc_equal, test_is_equal_different_scales);
    tcase_add_test(tc_equal, test_is_equal_different_signs);
    tcase_add_test(tc_equal, test_is_equal_different_values);
    tcase_add_test(tc_equal, test_is_equal_large_numbers);
    suite_add_tcase(s, tc_equal);

    TCase *tc_not_equal = tcase_create("is_not_equal");
    tcase_add_test(tc_not_equal, test_is_not_equal_basic);
    tcase_add_test(tc_not_equal, test_is_not_equal_same);
    tcase_add_test(tc_not_equal, test_is_not_equal_zeroes);
    tcase_add_test(tc_not_equal, test_is_not_equal_diff_signs);
    tcase_add_test(tc_not_equal, test_is_not_equal_diff_scales_same_val);
    suite_add_tcase(s, tc_not_equal);

    TCase *tc_less = tcase_create("is_less");
    tcase_add_test(tc_less, test_is_less_positive);
    tcase_add_test(tc_less, test_is_less_equal_values);
    tcase_add_test(tc_less, test_is_less_negatives);
    tcase_add_test(tc_less, test_is_less_mixed_signs);
    tcase_add_test(tc_less, test_is_less_zeroes);
    tcase_add_test(tc_less, test_is_less_zero_and_numbers);
    tcase_add_test(tc_less, test_is_less_different_scales);
    tcase_add_test(tc_less, test_is_less_different_scales_neg);
    tcase_add_test(tc_less, test_is_less_high_bits);
    suite_add_tcase(s, tc_less);

    TCase *tc_less_or_equal = tcase_create("is_less_or_equal");
    tcase_add_test(tc_less_or_equal, test_is_less_or_equal_basic);
    tcase_add_test(tc_less_or_equal, test_is_less_or_equal_zeroes);
    suite_add_tcase(s, tc_less_or_equal);

    TCase *tc_greater = tcase_create("is_greater");
    tcase_add_test(tc_greater, test_is_greater_positive);
    tcase_add_test(tc_greater, test_is_greater_equal);
    tcase_add_test(tc_greater, test_is_greater_negatives);
    tcase_add_test(tc_greater, test_is_greater_zeroes);
    tcase_add_test(tc_greater, test_is_greater_scales);
    suite_add_tcase(s, tc_greater);

    TCase *tc_greater_or_equal = tcase_create("is_greater_or_equal");
    tcase_add_test(tc_greater_or_equal, test_is_greater_or_equal_basic);
    tcase_add_test(tc_greater_or_equal, test_is_greater_or_equal_zeroes);
    suite_add_tcase(s, tc_greater_or_equal);

    return s;
}
