#include <limits.h>

#include "../../helpers/s21_utils.h"
#include "../../s21_decimal.h"
#include "../test_runner.h"

static void assert_bits(s21_decimal result, s21_decimal expected) {
  for (int i = 0; i < 4; i++) {
    ck_assert_uint_eq((unsigned int)result.bits[i],
                      (unsigned int)expected.bits[i]);
  }
}

static s21_decimal make_dec(unsigned int low, int sign) {
  s21_decimal value;
  init_decimal(&value);
  value.bits[0] = (int)low;
  set_sign(&value, sign);
  return value;
}

START_TEST(from_int_null) {
  ck_assert_int_eq(s21_from_int_to_decimal(1, NULL), 1);
}
END_TEST

START_TEST(from_int_zero) {
  s21_decimal result;
  s21_decimal expected;
  init_decimal(&expected);
  ck_assert_int_eq(s21_from_int_to_decimal(0, &result), 0);
  assert_bits(result, expected);
}
END_TEST

START_TEST(from_int_positive) {
  s21_decimal result;
  ck_assert_int_eq(s21_from_int_to_decimal(12345, &result), 0);
  assert_bits(result, make_dec(12345, 0));
}
END_TEST

START_TEST(from_int_negative) {
  s21_decimal result;
  ck_assert_int_eq(s21_from_int_to_decimal(-42, &result), 0);
  assert_bits(result, make_dec(42, 1));
}
END_TEST

START_TEST(from_int_limits) {
  s21_decimal result;
  ck_assert_int_eq(s21_from_int_to_decimal(INT_MAX, &result), 0);
  assert_bits(result, make_dec(2147483647U, 0));
  ck_assert_int_eq(s21_from_int_to_decimal(INT_MIN, &result), 0);
  assert_bits(result, make_dec(2147483648U, 1));
}
END_TEST

Suite *s21_from_int_to_decimal_suite(void) {
  Suite *suite = suite_create("s21_from_int_to_decimal");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, from_int_null);
  tcase_add_test(tc, from_int_zero);
  tcase_add_test(tc, from_int_positive);
  tcase_add_test(tc, from_int_negative);
  tcase_add_test(tc, from_int_limits);
  suite_add_tcase(suite, tc);
  return suite;
}
