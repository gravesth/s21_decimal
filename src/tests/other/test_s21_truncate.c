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

START_TEST(truncate_null) {
  s21_decimal value = make_dec(15, 0, 0, 1, 0);
  ck_assert_int_eq(s21_truncate(value, NULL), 1);
}
END_TEST

START_TEST(truncate_bad_scale) {
  s21_decimal value = make_dec(15, 0, 0, 0, 0);
  s21_decimal result = make_dec(1, 0, 0, 0, 0);
  value.bits[3] = 29 << 16;
  ck_assert_int_eq(s21_truncate(value, &result), 1);
  s21_decimal zero;
  init_decimal(&zero);
  assert_bits(result, zero);
}
END_TEST

START_TEST(truncate_integer) {
  s21_decimal value = make_dec(42, 0, 0, 0, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(value, &result), 0);
  assert_bits(result, value);
}
END_TEST

START_TEST(truncate_positive_fraction) {
  s21_decimal value = make_dec(1500, 0, 0, 3, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(value, &result), 0);
  assert_bits(result, make_dec(1, 0, 0, 0, 0));
}
END_TEST

START_TEST(truncate_negative_fraction) {
  s21_decimal value = make_dec(19, 0, 0, 1, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(value, &result), 0);
  assert_bits(result, make_dec(1, 0, 0, 0, 1));
}
END_TEST

START_TEST(truncate_fraction_to_zero) {
  s21_decimal value = make_dec(9, 0, 0, 1, 1);
  s21_decimal result;
  s21_decimal zero;
  init_decimal(&zero);
  ck_assert_int_eq(s21_truncate(value, &result), 0);
  assert_bits(result, zero);
}
END_TEST

START_TEST(truncate_drops_trailing_zeros) {
  s21_decimal value = make_dec(2500, 0, 0, 2, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(value, &result), 0);
  assert_bits(result, make_dec(25, 0, 0, 0, 1));
}
END_TEST

START_TEST(truncate_high_bits) {
  s21_decimal value = make_dec(6, 1, 1, 1, 1);
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(value, &result), 0);
  assert_bits(result, make_dec(3006477107U, 429496729U, 0, 0, 1));
}
END_TEST

Suite *s21_truncate_suite(void) {
  Suite *suite = suite_create("s21_truncate");
  TCase *tc = tcase_create("core");
  tcase_add_test(tc, truncate_null);
  tcase_add_test(tc, truncate_bad_scale);
  tcase_add_test(tc, truncate_integer);
  tcase_add_test(tc, truncate_positive_fraction);
  tcase_add_test(tc, truncate_negative_fraction);
  tcase_add_test(tc, truncate_fraction_to_zero);
  tcase_add_test(tc, truncate_drops_trailing_zeros);
  tcase_add_test(tc, truncate_high_bits);
  suite_add_tcase(suite, tc);
  return suite;
}
