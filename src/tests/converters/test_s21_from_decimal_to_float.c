#include <math.h>

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

START_TEST(to_float_null) {
  s21_decimal value = make_dec(1, 0, 0, 0, 0);
  ck_assert_int_eq(s21_from_decimal_to_float(value, NULL), 1);
}
END_TEST

START_TEST(to_float_bad_scale) {
  s21_decimal value = make_dec(1, 0, 0, 29, 0);
  float dst = 3.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(value, &dst), 1);
}
END_TEST

START_TEST(to_float_basic) {
  float dst = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(123, 0, 0, 0, 0), &dst),
                   0);
  ck_assert_float_eq(dst, 123.0f);
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(55, 0, 0, 1, 1), &dst),
                   0);
  ck_assert_float_eq_tol(dst, -5.5f, 1e-6);
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(1, 0, 0, 1, 0), &dst), 0);
  ck_assert_float_eq_tol(dst, 0.1f, 1e-6);
}
END_TEST

START_TEST(to_float_zeros) {
  float dst = 1.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(0, 0, 0, 0, 0), &dst), 0);
  ck_assert_float_eq(dst, 0.0f);
  ck_assert_int_eq(signbit(dst), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(0, 0, 0, 0, 1), &dst), 0);
  ck_assert_float_eq(dst, 0.0f);
  ck_assert_int_ne(signbit(dst), 0);
}
END_TEST

START_TEST(to_float_high_word) {
  float dst = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(0, 0, 1, 0, 0), &dst), 0);
  ck_assert_float_eq(dst, 18446744073709551616.0f);
}
END_TEST

START_TEST(to_float_middle_word) {
  float dst = 0.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(make_dec(0, 1, 0, 0, 1), &dst), 0);
  ck_assert_float_eq(dst, -4294967296.0f);
}
END_TEST

Suite *s21_from_decimal_to_float_suite(void) {
  Suite *suite = suite_create("s21_from_decimal_to_float");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, to_float_null);
  tcase_add_test(tc, to_float_bad_scale);
  tcase_add_test(tc, to_float_basic);
  tcase_add_test(tc, to_float_zeros);
  tcase_add_test(tc, to_float_high_word);
  tcase_add_test(tc, to_float_middle_word);
  suite_add_tcase(suite, tc);
  return suite;
}
