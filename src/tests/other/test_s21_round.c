#include "../../helpers/s21_utils.h"
#include "../../s21_decimal.h"
#include "../test_runner.h"

static void assert_bits(s21_decimal result, s21_decimal expected) {
  for (int i = 0; i < 4; i++) {
    ck_assert_uint_eq((unsigned int)result.bits[i],
                      (unsigned int)expected.bits[i]);
  }
}

static s21_decimal make_dec(unsigned int low, unsigned int mid, int scale,
                            int sign) {
  s21_decimal value;
  init_decimal(&value);
  value.bits[0] = (int)low;
  value.bits[1] = (int)mid;
  set_scale(&value, scale);
  set_sign(&value, sign);
  return value;
}

static void expect_round(unsigned int low, int scale, int sign,
                         unsigned int exp_low, int exp_sign) {
  s21_decimal result;
  ck_assert_int_eq(s21_round(make_dec(low, 0, scale, sign), &result), 0);
  assert_bits(result, make_dec(exp_low, 0, 0, exp_sign));
}

START_TEST(round_null) {
  s21_decimal value = make_dec(15, 0, 1, 0);
  ck_assert_int_eq(s21_round(value, NULL), 1);
}
END_TEST

START_TEST(round_bad_scale) {
  s21_decimal value = make_dec(15, 0, 0, 0);
  s21_decimal result;
  s21_decimal zero;
  init_decimal(&zero);
  value.bits[3] = 29 << 16;
  ck_assert_int_eq(s21_round(value, &result), 1);
  assert_bits(result, zero);
}
END_TEST

START_TEST(round_integer) {
  s21_decimal value = make_dec(25, 0, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  assert_bits(result, value);
}
END_TEST

START_TEST(round_half_away) {
  expect_round(4, 1, 0, 0, 0);
  expect_round(5, 1, 0, 1, 0);
  expect_round(6, 1, 0, 1, 0);
  expect_round(15, 1, 0, 2, 0);
  expect_round(25, 1, 0, 3, 0);
  expect_round(149, 2, 0, 1, 0);
  expect_round(151, 2, 0, 2, 0);
  expect_round(4, 1, 1, 0, 0);
  expect_round(5, 1, 1, 1, 1);
  expect_round(15, 1, 1, 2, 1);
  expect_round(25, 1, 1, 3, 1);
}
END_TEST

START_TEST(round_wide_mantissa) {
  s21_decimal value = make_dec(0, 1, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_round(value, &result), 0);
  assert_bits(result, make_dec(429496730U, 0, 0, 0));
}
END_TEST

Suite *s21_round_suite(void) {
  Suite *suite = suite_create("s21_round");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, round_null);
  tcase_add_test(tc, round_bad_scale);
  tcase_add_test(tc, round_integer);
  tcase_add_test(tc, round_half_away);
  tcase_add_test(tc, round_wide_mantissa);
  suite_add_tcase(suite, tc);
  return suite;
}
