#include <limits.h>

#include "../../helpers/s21_utils.h"
#include "../../s21_decimal.h"
#include "../test_runner.h"

static s21_decimal make_dec(unsigned int low, unsigned int mid,
                            unsigned int high, int scale, int sign) {
  s21_decimal value;
  init_decimal(&value);
  value.bits[0] = (int)low;
  value.bits[1] = (int)mid;
  value.bits[2] = (int)high;
  if (scale <= 28) {
    set_scale(&value, scale);
  } else {
    value.bits[3] = scale << 16;
  }
  set_sign(&value, sign);
  return value;
}

START_TEST(to_int_null) {
  s21_decimal value = make_dec(1, 0, 0, 0, 0);
  ck_assert_int_eq(s21_from_decimal_to_int(value, NULL), 1);
}
END_TEST

START_TEST(to_int_bad_scale) {
  s21_decimal value = make_dec(1, 0, 0, 29, 0);
  int dst = 7;
  ck_assert_int_eq(s21_from_decimal_to_int(value, &dst), 1);
}
END_TEST

START_TEST(to_int_basic) {
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(42, 0, 0, 0, 0), &dst), 0);
  ck_assert_int_eq(dst, 42);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(42, 0, 0, 0, 1), &dst), 0);
  ck_assert_int_eq(dst, -42);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(0, 0, 0, 0, 0), &dst), 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(to_int_truncates_fraction) {
  int dst = 0;
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(19, 0, 0, 1, 0), &dst), 0);
  ck_assert_int_eq(dst, 1);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(19, 0, 0, 1, 1), &dst), 0);
  ck_assert_int_eq(dst, -1);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(9, 0, 0, 1, 0), &dst), 0);
  ck_assert_int_eq(dst, 0);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(9, 0, 0, 1, 1), &dst), 0);
  ck_assert_int_eq(dst, 0);
}
END_TEST

START_TEST(to_int_limits) {
  int dst = 0;
  ck_assert_int_eq(
      s21_from_decimal_to_int(make_dec(2147483647U, 0, 0, 0, 0), &dst), 0);
  ck_assert_int_eq(dst, INT_MAX);
  ck_assert_int_eq(
      s21_from_decimal_to_int(make_dec(2147483648U, 0, 0, 0, 1), &dst), 0);
  ck_assert_int_eq(dst, INT_MIN);
}
END_TEST

START_TEST(to_int_overflow) {
  int dst = 5;
  ck_assert_int_eq(
      s21_from_decimal_to_int(make_dec(2147483648U, 0, 0, 0, 0), &dst), 1);
  ck_assert_int_eq(
      s21_from_decimal_to_int(make_dec(2147483649U, 0, 0, 0, 1), &dst), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(0, 1, 0, 0, 0), &dst), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(make_dec(0, 0, 1, 0, 1), &dst), 1);
}
END_TEST

Suite *s21_from_decimal_to_int_suite(void) {
  Suite *suite = suite_create("s21_from_decimal_to_int");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, to_int_null);
  tcase_add_test(tc, to_int_bad_scale);
  tcase_add_test(tc, to_int_basic);
  tcase_add_test(tc, to_int_truncates_fraction);
  tcase_add_test(tc, to_int_limits);
  tcase_add_test(tc, to_int_overflow);
  suite_add_tcase(suite, tc);
  return suite;
}
