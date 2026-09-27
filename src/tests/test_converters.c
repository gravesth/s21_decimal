#include <math.h>

#include "../helpers/s21_utils.h"
#include "test_runner.h"

static s21_decimal make_decimal(uint32_t low, uint32_t mid, uint32_t high,
                                int scale, int sign) {
  s21_decimal value;
  s21_init_decimal(&value);
  value.bits[0] = (int)low;
  value.bits[1] = (int)mid;
  value.bits[2] = (int)high;
  s21_set_scale(&value, scale);
  s21_set_sign(&value, sign);
  return value;
}

START_TEST(from_int_basic) {
  s21_decimal value;
  ck_assert_int_eq(s21_from_int_to_decimal(0, &value), 0);
  ck_assert_int_eq(s21_is_zero(value), 1);
  ck_assert_int_eq(s21_get_sign(value), 0);

  ck_assert_int_eq(s21_from_int_to_decimal(42, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(value), 0);

  ck_assert_int_eq(s21_from_int_to_decimal(-42, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 42);
  ck_assert_int_eq(s21_get_sign(value), 1);
}
END_TEST

START_TEST(from_int_limits) {
  s21_decimal value;
  ck_assert_int_eq(s21_from_int_to_decimal(2147483647, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 2147483647u);
  ck_assert_int_eq(s21_get_sign(value), 0);

  ck_assert_int_eq(s21_from_int_to_decimal(-2147483647 - 1, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 2147483648u);
  ck_assert_int_eq(s21_get_sign(value), 1);
  ck_assert_int_eq(s21_from_int_to_decimal(1, NULL), 1);
}
END_TEST

START_TEST(to_int_truncates_fraction) {
  int result = 99;
  s21_decimal value = make_decimal(19, 0, 0, 1, 0);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 0);
  ck_assert_int_eq(result, 1);

  value = make_decimal(19, 0, 0, 1, 1);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 0);
  ck_assert_int_eq(result, -1);

  value = make_decimal(0, 0, 0, 5, 1);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(to_int_limits_and_errors) {
  int result = 0;
  s21_decimal value = make_decimal(2147483647u, 0, 0, 0, 0);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 0);
  ck_assert_int_eq(result, 2147483647);

  value = make_decimal(2147483648u, 0, 0, 0, 1);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 0);
  ck_assert_int_eq(result, -2147483647 - 1);

  value = make_decimal(2147483648u, 0, 0, 0, 0);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 1);

  value = make_decimal(2147483649u, 0, 0, 0, 1);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 1);

  value = make_decimal(1, 1, 0, 0, 0);
  ck_assert_int_eq(s21_from_decimal_to_int(value, &result), 1);
  ck_assert_int_eq(s21_from_decimal_to_int(value, NULL), 1);
}
END_TEST

START_TEST(float_roundtrip_simple) {
  s21_decimal value;
  float result = 0.0f;
  ck_assert_int_eq(s21_from_float_to_decimal(0.0f, &value), 0);
  ck_assert_int_eq(s21_is_zero(value), 1);
  ck_assert_int_eq(s21_get_sign(value), 0);
  ck_assert_int_eq(s21_from_decimal_to_float(value, &result), 0);
  ck_assert_float_eq(result, 0.0f);

  ck_assert_int_eq(s21_from_float_to_decimal(-0.0f, &value), 0);
  ck_assert_int_eq(s21_get_sign(value), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(value, &result), 0);
  ck_assert(signbit(result));

  ck_assert_int_eq(s21_from_float_to_decimal(1.25f, &value), 0);
  s21_decimal expected = make_decimal(125, 0, 0, 2, 0);
  ck_assert_int_eq(s21_is_equal(value, expected), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(value, &result), 0);
  ck_assert_float_eq_tol(result, 1.25f, 1e-6f);

  ck_assert_int_eq(s21_from_float_to_decimal(-42.0f, &value), 0);
  ck_assert_int_eq(s21_get_sign(value), 1);
  ck_assert_int_eq(s21_from_decimal_to_float(value, &result), 0);
  ck_assert_float_eq(result, -42.0f);
}
END_TEST

START_TEST(float_bankers_seven_digits) {
  s21_decimal value;
  ck_assert_int_eq(s21_from_float_to_decimal(12345625.0f, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 12345620u);
  ck_assert_int_eq(s21_get_scale(value), 0);

  ck_assert_int_eq(s21_from_float_to_decimal(12345635.0f, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 12345640u);

  ck_assert_int_eq(s21_from_float_to_decimal(12345675.0f, &value), 0);
  ck_assert_uint_eq((uint32_t)value.bits[0], 12345680u);
}
END_TEST

START_TEST(float_range_errors) {
  s21_decimal value;
  value.bits[0] = 5;
  ck_assert_int_eq(s21_from_float_to_decimal(1e-29f, &value), 1);
  ck_assert_int_eq(s21_is_zero(value), 1);

  ck_assert_int_eq(s21_from_float_to_decimal(1e-28f, &value), 0);
  s21_decimal tiny = make_decimal(1, 0, 0, 28, 0);
  ck_assert_int_eq(s21_is_equal(value, tiny), 1);

  ck_assert_int_eq(s21_from_float_to_decimal(0x1p96f, &value), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(-0x1p96f, &value), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(INFINITY, &value), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(-INFINITY, &value), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(NAN, &value), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(1.0f, NULL), 1);

  float output = 1.0f;
  ck_assert_int_eq(s21_from_decimal_to_float(tiny, NULL), 1);
  s21_decimal wide = make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 1);
  ck_assert_int_eq(s21_from_decimal_to_float(wide, &output), 0);
  ck_assert(output < 0.0f);
  ck_assert(isfinite(output));
}
END_TEST

Suite *s21_converters_suite(void) {
  Suite *suite = suite_create("s21_converters");
  TCase *core = tcase_create("core");
  tcase_add_test(core, from_int_basic);
  tcase_add_test(core, from_int_limits);
  tcase_add_test(core, to_int_truncates_fraction);
  tcase_add_test(core, to_int_limits_and_errors);
  tcase_add_test(core, float_roundtrip_simple);
  tcase_add_test(core, float_bankers_seven_digits);
  tcase_add_test(core, float_range_errors);
  suite_add_tcase(suite, core);
  return suite;
}
