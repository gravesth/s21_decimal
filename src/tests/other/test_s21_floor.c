#include "../../helpers/s21_utils.h"
#include "../../s21_decimal.h"
#include "../test_runner.h"

static void assert_bits(s21_decimal result, s21_decimal expected) {
  for (int i = 0; i < 4; i++) {
    ck_assert_uint_eq((unsigned int)result.bits[i],
                      (unsigned int)expected.bits[i]);
  }
}

static s21_decimal make_dec(unsigned int low, int scale, int sign) {
  s21_decimal value;
  init_decimal(&value);
  value.bits[0] = (int)low;
  set_scale(&value, scale);
  set_sign(&value, sign);
  return value;
}

START_TEST(floor_null) {
  s21_decimal value = make_dec(15, 1, 0);
  ck_assert_int_eq(s21_floor(value, NULL), 1);
}
END_TEST

START_TEST(floor_bad_scale) {
  s21_decimal value = make_dec(15, 0, 0);
  s21_decimal result;
  s21_decimal zero;
  init_decimal(&zero);
  value.bits[3] = 29 << 16;
  ck_assert_int_eq(s21_floor(value, &result), 1);
  assert_bits(result, zero);
}
END_TEST

START_TEST(floor_positive_fraction) {
  s21_decimal result;
  ck_assert_int_eq(s21_floor(make_dec(11, 1, 0), &result), 0);
  assert_bits(result, make_dec(1, 0, 0));
}
END_TEST

START_TEST(floor_positive_integer) {
  s21_decimal value = make_dec(5, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_floor(value, &result), 0);
  assert_bits(result, value);
}
END_TEST

START_TEST(floor_negative_integer) {
  s21_decimal value = make_dec(5, 0, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_floor(value, &result), 0);
  assert_bits(result, value);
}
END_TEST

START_TEST(floor_negative_with_trailing_zeros) {
  s21_decimal result;
  ck_assert_int_eq(s21_floor(make_dec(200, 2, 1), &result), 0);
  assert_bits(result, make_dec(2, 0, 1));
}
END_TEST

START_TEST(floor_negative_fraction) {
  s21_decimal result;
  ck_assert_int_eq(s21_floor(make_dec(11, 1, 1), &result), 0);
  assert_bits(result, make_dec(2, 0, 1));
}
END_TEST

START_TEST(floor_negative_only_fraction) {
  s21_decimal result;
  ck_assert_int_eq(s21_floor(make_dec(1, 1, 1), &result), 0);
  assert_bits(result, make_dec(1, 0, 1));
}
END_TEST

Suite *s21_floor_suite(void) {
  Suite *suite = suite_create("s21_floor");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, floor_null);
  tcase_add_test(tc, floor_bad_scale);
  tcase_add_test(tc, floor_positive_fraction);
  tcase_add_test(tc, floor_positive_integer);
  tcase_add_test(tc, floor_negative_integer);
  tcase_add_test(tc, floor_negative_with_trailing_zeros);
  tcase_add_test(tc, floor_negative_fraction);
  tcase_add_test(tc, floor_negative_only_fraction);
  suite_add_tcase(suite, tc);
  return suite;
}
