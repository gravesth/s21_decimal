#include <math.h>

#include "../../helpers/s21_utils.h"
#include "../../s21_decimal.h"
#include "../test_runner.h"

static void assert_bits(s21_decimal result, s21_decimal expected) {
  for (int i = 0; i < 4; i++) {
    ck_assert_uint_eq((unsigned int)result.bits[i],
                      (unsigned int)expected.bits[i]);
  }
}

static s21_decimal make_dec(unsigned int low, unsigned int mid,
                            unsigned int high, int scale, int sign) {
  s21_decimal value;
  init_decimal(&value);
  value.bits[0] = (int)low;
  value.bits[1] = (int)mid;
  value.bits[2] = (int)high;
  set_scale(&value, scale);
  set_sign(&value, sign);
  return value;
}

static void expect_float(float src, s21_decimal expected) {
  s21_decimal result;
  ck_assert_int_eq(s21_from_float_to_decimal(src, &result), 0);
  assert_bits(result, expected);
}

START_TEST(from_float_null) {
  ck_assert_int_eq(s21_from_float_to_decimal(1.0f, NULL), 1);
}
END_TEST

START_TEST(from_float_zeros) {
  s21_decimal result;
  s21_decimal positive;
  s21_decimal negative;
  init_decimal(&positive);
  init_decimal(&negative);
  set_sign(&negative, 1);
  ck_assert_int_eq(s21_from_float_to_decimal(0.0f, &result), 0);
  assert_bits(result, positive);
  ck_assert_int_eq(s21_from_float_to_decimal(-0.0f, &result), 0);
  assert_bits(result, negative);
}
END_TEST

START_TEST(from_float_errors) {
  s21_decimal result = make_dec(1, 0, 0, 0, 0);
  s21_decimal zero;
  init_decimal(&zero);
  ck_assert_int_eq(s21_from_float_to_decimal(nanf(""), &result), 1);
  assert_bits(result, zero);
  ck_assert_int_eq(s21_from_float_to_decimal(INFINITY, &result), 1);
  assert_bits(result, zero);
  ck_assert_int_eq(s21_from_float_to_decimal(-INFINITY, &result), 1);
  assert_bits(result, zero);
  ck_assert_int_eq(s21_from_float_to_decimal(1e-29f, &result), 1);
  assert_bits(result, zero);
  ck_assert_int_eq(s21_from_float_to_decimal(-1e-29f, &result), 1);
  assert_bits(result, zero);
  ck_assert_int_eq(s21_from_float_to_decimal(7.922817e28f, &result), 1);
  assert_bits(result, zero);
}
END_TEST

START_TEST(from_float_exact_values) {
  expect_float(1.5f, make_dec(15, 0, 0, 1, 0));
  expect_float(-1.5f, make_dec(15, 0, 0, 1, 1));
  expect_float(0.1f, make_dec(1, 0, 0, 1, 0));
  expect_float(1.23456789f, make_dec(1234568, 0, 0, 6, 0));
  expect_float(1234567.0f, make_dec(1234567, 0, 0, 0, 0));
  expect_float(12345678.0f, make_dec(12345680, 0, 0, 0, 0));
  expect_float(1e-28f, make_dec(1, 0, 0, 28, 0));
  expect_float(4.5e-28f, make_dec(4, 0, 0, 28, 0));
  expect_float(9.5e-28f, make_dec(1, 0, 0, 27, 0));
  expect_float(1.23e-27f, make_dec(12, 0, 0, 28, 0));
  expect_float(1.23e20f, make_dec(1829502976U, 2868365393U, 6, 0, 0));
  expect_float(7.922816e28f,
               make_dec(536870912U, 3012735514U, 4294967159U, 0, 0));
}
END_TEST

Suite *s21_from_float_to_decimal_suite(void) {
  Suite *suite = suite_create("s21_from_float_to_decimal");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, from_float_null);
  tcase_add_test(tc, from_float_zeros);
  tcase_add_test(tc, from_float_errors);
  tcase_add_test(tc, from_float_exact_values);
  suite_add_tcase(suite, tc);
  return suite;
}
