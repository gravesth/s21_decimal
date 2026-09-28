#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"
#include "test_runner.h"

START_TEST(helpers_null_arguments) {
  s21_decimal value;
  s21_big_decimal big;
  init_decimal(&value);
  init_big_decimal(&big);
  init_decimal(NULL);
  init_big_decimal(NULL);
  get_big_decimal(value, NULL);
  add_process(big, big, NULL);
  sub_process(big, big, NULL);
  ck_assert_int_eq(value.bits[0], 0);
}
END_TEST

START_TEST(set_bit_ignored_arguments) {
  s21_decimal value;
  init_decimal(&value);
  value.bits[0] = 1;
  set_bit(NULL, 0, 1);
  set_bit(&value, -1, 1);
  set_bit(&value, 128, 1);
  set_bit(&value, 0, 2);
  ck_assert_uint_eq((unsigned int)value.bits[0], 1U);
}
END_TEST

START_TEST(set_bit_writes_and_clears) {
  s21_decimal value;
  init_decimal(&value);
  set_bit(&value, 0, 1);
  set_bit(&value, 31, 1);
  set_bit(&value, 32, 1);
  set_bit(&value, 127, 1);
  ck_assert_int_eq(get_bit(value, 0), 1);
  ck_assert_int_eq(get_bit(value, 31), 1);
  ck_assert_int_eq(get_bit(value, 32), 1);
  ck_assert_int_eq(get_bit(value, 127), 1);
  set_bit(&value, 0, 0);
  set_bit(&value, 31, 0);
  set_bit(&value, 127, 0);
  ck_assert_int_eq(get_bit(value, 0), 0);
  ck_assert_int_eq(get_bit(value, 31), 0);
  ck_assert_int_eq(get_bit(value, 32), 1);
  ck_assert_int_eq(get_bit(value, 127), 0);
}
END_TEST

Suite *s21_helpers_suite(void) {
  Suite *suite = suite_create("s21_helpers");
  TCase *tc = tcase_create("set_bit");
  tcase_add_test(tc, helpers_null_arguments);
  tcase_add_test(tc, set_bit_ignored_arguments);
  tcase_add_test(tc, set_bit_writes_and_clears);
  suite_add_tcase(suite, tc);
  return suite;
}
