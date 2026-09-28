#include "../../helpers/s21_utils.h"
#include "../../s21_decimal.h"
#include "../test_runner.h"

static void assert_bits(s21_decimal result, s21_decimal expected) {
  for (int i = 0; i < 4; i++) {
    ck_assert_uint_eq((unsigned int)result.bits[i],
                      (unsigned int)expected.bits[i]);
  }
}

START_TEST(negate_null) {
  s21_decimal value = {{5, 0, 0, 0}};
  ck_assert_int_eq(s21_negate(value, NULL), 1);
}
END_TEST

START_TEST(negate_positive) {
  s21_decimal value;
  s21_decimal result;
  init_decimal(&value);
  value.bits[0] = 42;
  set_scale(&value, 2);
  ck_assert_int_eq(s21_negate(value, &result), 0);
  set_sign(&value, 1);
  assert_bits(result, value);
}
END_TEST

START_TEST(negate_negative) {
  s21_decimal value;
  s21_decimal result;
  init_decimal(&value);
  value.bits[0] = 42;
  set_sign(&value, 1);
  ck_assert_int_eq(s21_negate(value, &result), 0);
  set_sign(&value, 0);
  assert_bits(result, value);
}
END_TEST

START_TEST(negate_zero) {
  s21_decimal value;
  s21_decimal result;
  s21_decimal expected;
  init_decimal(&value);
  init_decimal(&expected);
  set_sign(&expected, 1);
  ck_assert_int_eq(s21_negate(value, &result), 0);
  assert_bits(result, expected);
}
END_TEST

START_TEST(negate_negative_zero) {
  s21_decimal value;
  s21_decimal result;
  s21_decimal expected;
  init_decimal(&value);
  init_decimal(&expected);
  set_sign(&value, 1);
  ck_assert_int_eq(s21_negate(value, &result), 0);
  assert_bits(result, expected);
}
END_TEST

Suite *s21_negate_suite(void) {
  Suite *suite = suite_create("s21_negate");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, negate_null);
  tcase_add_test(tc, negate_positive);
  tcase_add_test(tc, negate_negative);
  tcase_add_test(tc, negate_zero);
  tcase_add_test(tc, negate_negative_zero);
  suite_add_tcase(suite, tc);
  return suite;
}
