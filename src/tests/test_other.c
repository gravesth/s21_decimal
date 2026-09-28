#include <check.h>
#include <limits.h>

#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"
#include "test_runner.h"

/* ========================================================================== */
/*                                s21_truncate                                */
/* ========================================================================== */

START_TEST(test_truncate_positive_simple) {
  s21_decimal src = {{123456, 0, 0, 3 << 16}}; // 123.456
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 123);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_truncate_negative_simple) {
  s21_decimal src = {{123456, 0, 0, (int)0x80030000}}; // -123.456
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 123);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_truncate_fraction_only) {
  s21_decimal src = {{999, 0, 0, 3 << 16}}; // 0.999
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_truncate_negative_fraction_only) {
  s21_decimal src = {{999, 0, 0, (int)0x80030000}}; // -0.999
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_truncate_already_integer) {
  s21_decimal src = {{77, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 77);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_truncate_trailing_zeros) {
  s21_decimal src = {{1500, 0, 0, 3 << 16}}; // 1.500
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 1);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_truncate_zero) {
  s21_decimal src = {{0, 0, 0, 5 << 16}}; // 0.00000
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[1], 0);
  ck_assert_int_eq(dst.bits[2], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_truncate_large_number) {
  // Number spanning multiple 32-bit words: 100000000000000.5
  s21_decimal src;
  init_decimal(&src);
  src.bits[0] = (int)0xA4C68005; // 1000000000000005 in hex
  src.bits[1] = 0x00038D7E;
  set_scale(&src, 1);
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(get_scale(dst), 0);
  // dst should be 100000000000000 (0x5AF3107A4000)
  ck_assert_int_eq(dst.bits[0], 0x107A4000);
  ck_assert_int_eq(dst.bits[1], 0x00005AF3);
  ck_assert_int_eq(dst.bits[2], 0);
}
END_TEST

START_TEST(test_truncate_max_decimal) {
  s21_decimal src = {{(int)0xFFFFFFFF, (int)0xFFFFFFFF, (int)0xFFFFFFFF, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], (int)0xFFFFFFFF);
  ck_assert_int_eq(dst.bits[1], (int)0xFFFFFFFF);
  ck_assert_int_eq(dst.bits[2], (int)0xFFFFFFFF);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_truncate_null) {
  s21_decimal src = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_truncate(src, NULL), 1);
}
END_TEST

START_TEST(test_truncate_invalid_scale) {
  s21_decimal src = {{1, 0, 0, 29 << 16}};
  s21_decimal dst;
  ck_assert_int_eq(s21_truncate(src, &dst), 1);
}
END_TEST

/* ========================================================================== */
/*                                  s21_floor                                 */
/* ========================================================================== */

START_TEST(test_floor_positive) {
  s21_decimal src = {{57, 0, 0, 1 << 16}}; // 5.7
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 5);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_floor_positive_exact) {
  s21_decimal src = {{50, 0, 0, 1 << 16}}; // 5.0
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 5);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_floor_positive_small) {
  s21_decimal src = {{9, 0, 0, 1 << 16}}; // 0.9
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_floor_negative) {
  s21_decimal src = {{57, 0, 0, (int)0x80010000}}; // -5.7
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 6);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000); // -6
}
END_TEST

START_TEST(test_floor_negative_exact) {
  s21_decimal src = {{50, 0, 0, (int)0x80010000}}; // -5.0
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 5);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000); // -5
}
END_TEST

START_TEST(test_floor_negative_fraction_only) {
  s21_decimal src = {{1, 0, 0, (int)0x80010000}}; // -0.1
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 1);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000); // -1
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_floor_already_integer) {
  s21_decimal src = {{42, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 42);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_floor_null) {
  s21_decimal src = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_floor(src, NULL), 1);
}
END_TEST

START_TEST(test_floor_invalid_scale) {
  s21_decimal src = {{1, 0, 0, 29 << 16}};
  s21_decimal dst;
  ck_assert_int_eq(s21_floor(src, &dst), 1);
}
END_TEST

/* ========================================================================== */
/*                                  s21_round                                 */
/* ========================================================================== */

START_TEST(test_round_half_to_even_down) {
  s21_decimal src = {{25, 0, 0, 1 << 16}}; // 2.5 -> 2
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 2);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_half_to_even_up) {
  s21_decimal src = {{35, 0, 0, 1 << 16}}; // 3.5 -> 4
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 4);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_negative_half_to_even_up) {
  s21_decimal src = {{25, 0, 0, (int)0x80010000}}; // -2.5 -> -2
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 2);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_round_negative_half_to_even_down) {
  s21_decimal src = {{35, 0, 0, (int)0x80010000}}; // -3.5 -> -4
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 4);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_round_greater_than_half) {
  s21_decimal src = {{251, 0, 0, 2 << 16}}; // 2.51 -> 3
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 3);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_less_than_half) {
  s21_decimal src = {{249, 0, 0, 2 << 16}}; // 2.49 -> 2
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 2);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_negative_greater) {
  s21_decimal src = {{251, 0, 0, (int)0x80020000}}; // -2.51 -> -3
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 3);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_round_negative_less) {
  s21_decimal src = {{249, 0, 0, (int)0x80020000}}; // -2.49 -> -2
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 2);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_round_zero_point_five) {
  s21_decimal src = {{5, 0, 0, 1 << 16}}; // 0.5 -> 0
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_one_point_five) {
  s21_decimal src = {{15, 0, 0, 1 << 16}}; // 1.5 -> 2
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 2);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_already_integer) {
  s21_decimal src = {{42, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 42);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_round_null) {
  s21_decimal src = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_round(src, NULL), 1);
}
END_TEST

START_TEST(test_round_invalid_scale) {
  s21_decimal src = {{1, 0, 0, 29 << 16}};
  s21_decimal dst;
  ck_assert_int_eq(s21_round(src, &dst), 1);
}
END_TEST

/* ========================================================================== */
/*                                 s21_negate                                 */
/* ========================================================================== */

START_TEST(test_negate_positive_to_negative) {
  s21_decimal src = {{42, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_negate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 42);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_negate_negative_to_positive) {
  s21_decimal src = {{42, 0, 0, (int)0x80000000}};
  s21_decimal dst;
  ck_assert_int_eq(s21_negate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 42);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal src = {{0, 0, 0, 0}};
  s21_decimal dst;
  ck_assert_int_eq(s21_negate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[3], (int)0x80000000);
}
END_TEST

START_TEST(test_negate_negative_zero) {
  s21_decimal src = {{0, 0, 0, (int)0x80000000}};
  s21_decimal dst;
  ck_assert_int_eq(s21_negate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 0);
  ck_assert_int_eq(dst.bits[3], 0);
}
END_TEST

START_TEST(test_negate_preserves_scale) {
  s21_decimal src = {{123, 0, 0, 2 << 16}};
  s21_decimal dst;
  ck_assert_int_eq(s21_negate(src, &dst), 0);
  ck_assert_int_eq(dst.bits[0], 123);
  ck_assert_int_eq(dst.bits[3], (int)0x80020000);
}
END_TEST

START_TEST(test_negate_double) {
  s21_decimal src = {{777, 0, 0, 3 << 16}};
  s21_decimal tmp, dst;
  ck_assert_int_eq(s21_negate(src, &tmp), 0);
  ck_assert_int_eq(s21_negate(tmp, &dst), 0);
  ck_assert_int_eq(dst.bits[0], src.bits[0]);
  ck_assert_int_eq(dst.bits[3], src.bits[3]);
}
END_TEST

START_TEST(test_negate_null) {
  s21_decimal src = {{1, 0, 0, 0}};
  ck_assert_int_eq(s21_negate(src, NULL), 1);
}
END_TEST

/* ========================================================================== */
/*                               Suite Setup                                  */
/* ========================================================================== */

Suite *suite_other(void) {
  Suite *s = suite_create("s21_other");

  TCase *tc_truncate = tcase_create("truncate");
  tcase_add_test(tc_truncate, test_truncate_positive_simple);
  tcase_add_test(tc_truncate, test_truncate_negative_simple);
  tcase_add_test(tc_truncate, test_truncate_fraction_only);
  tcase_add_test(tc_truncate, test_truncate_negative_fraction_only);
  tcase_add_test(tc_truncate, test_truncate_already_integer);
  tcase_add_test(tc_truncate, test_truncate_trailing_zeros);
  tcase_add_test(tc_truncate, test_truncate_zero);
  tcase_add_test(tc_truncate, test_truncate_large_number);
  tcase_add_test(tc_truncate, test_truncate_max_decimal);
  tcase_add_test(tc_truncate, test_truncate_null);
  tcase_add_test(tc_truncate, test_truncate_invalid_scale);
  suite_add_tcase(s, tc_truncate);

  TCase *tc_floor = tcase_create("floor");
  tcase_add_test(tc_floor, test_floor_positive);
  tcase_add_test(tc_floor, test_floor_positive_exact);
  tcase_add_test(tc_floor, test_floor_positive_small);
  tcase_add_test(tc_floor, test_floor_negative);
  tcase_add_test(tc_floor, test_floor_negative_exact);
  tcase_add_test(tc_floor, test_floor_negative_fraction_only);
  tcase_add_test(tc_floor, test_floor_zero);
  tcase_add_test(tc_floor, test_floor_already_integer);
  tcase_add_test(tc_floor, test_floor_null);
  tcase_add_test(tc_floor, test_floor_invalid_scale);
  suite_add_tcase(s, tc_floor);

  TCase *tc_round = tcase_create("round");
  tcase_add_test(tc_round, test_round_half_to_even_down);
  tcase_add_test(tc_round, test_round_half_to_even_up);
  tcase_add_test(tc_round, test_round_negative_half_to_even_up);
  tcase_add_test(tc_round, test_round_negative_half_to_even_down);
  tcase_add_test(tc_round, test_round_greater_than_half);
  tcase_add_test(tc_round, test_round_less_than_half);
  tcase_add_test(tc_round, test_round_negative_greater);
  tcase_add_test(tc_round, test_round_negative_less);
  tcase_add_test(tc_round, test_round_zero_point_five);
  tcase_add_test(tc_round, test_round_one_point_five);
  tcase_add_test(tc_round, test_round_zero);
  tcase_add_test(tc_round, test_round_already_integer);
  tcase_add_test(tc_round, test_round_null);
  tcase_add_test(tc_round, test_round_invalid_scale);
  suite_add_tcase(s, tc_round);

  TCase *tc_negate = tcase_create("negate");
  tcase_add_test(tc_negate, test_negate_positive_to_negative);
  tcase_add_test(tc_negate, test_negate_negative_to_positive);
  tcase_add_test(tc_negate, test_negate_zero);
  tcase_add_test(tc_negate, test_negate_negative_zero);
  tcase_add_test(tc_negate, test_negate_preserves_scale);
  tcase_add_test(tc_negate, test_negate_double);
  tcase_add_test(tc_negate, test_negate_null);
  suite_add_tcase(s, tc_negate);

  return s;
}
