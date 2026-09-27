#include "../helpers/s21_utils.h"
#include "test_runner.h"

static s21_decimal make_decimal(uint32_t low, int scale, int sign) {
  s21_decimal value;
  s21_init_decimal(&value);
  value.bits[0] = (int)low;
  s21_set_scale(&value, scale);
  s21_set_sign(&value, sign);
  return value;
}

static void assert_bits(s21_decimal value, uint32_t low, int scale, int sign) {
  ck_assert_uint_eq((uint32_t)value.bits[0], low);
  ck_assert_uint_eq((uint32_t)value.bits[1], 0);
  ck_assert_uint_eq((uint32_t)value.bits[2], 0);
  ck_assert_int_eq(s21_get_scale(value), scale);
  ck_assert_int_eq(s21_get_sign(value), sign);
}

START_TEST(truncate_drops_fraction) {
  s21_decimal result;
  ck_assert_int_eq(s21_truncate(make_decimal(19, 1, 0), &result), 0);
  assert_bits(result, 1, 0, 0);
  ck_assert_int_eq(s21_truncate(make_decimal(19, 1, 1), &result), 0);
  assert_bits(result, 1, 0, 1);
  ck_assert_int_eq(s21_truncate(make_decimal(1500, 3, 0), &result), 0);
  assert_bits(result, 1, 0, 0);
  ck_assert_int_eq(s21_truncate(make_decimal(7, 0, 1), &result), 0);
  assert_bits(result, 7, 0, 1);
  ck_assert_int_eq(s21_truncate(make_decimal(0, 5, 1), &result), 0);
  assert_bits(result, 0, 0, 0);
  ck_assert_int_eq(s21_truncate(make_decimal(1, 0, 0), NULL), 1);
}
END_TEST

START_TEST(floor_toward_minus_inf) {
  s21_decimal result;
  ck_assert_int_eq(s21_floor(make_decimal(19, 1, 0), &result), 0);
  assert_bits(result, 1, 0, 0);
  ck_assert_int_eq(s21_floor(make_decimal(20, 1, 0), &result), 0);
  assert_bits(result, 2, 0, 0);
  ck_assert_int_eq(s21_floor(make_decimal(11, 1, 1), &result), 0);
  assert_bits(result, 2, 0, 1);
  ck_assert_int_eq(s21_floor(make_decimal(20, 1, 1), &result), 0);
  assert_bits(result, 2, 0, 1);
  ck_assert_int_eq(s21_floor(make_decimal(1, 1, 1), &result), 0);
  assert_bits(result, 1, 0, 1);
  ck_assert_int_eq(s21_floor(make_decimal(0, 0, 0), NULL), 1);
}
END_TEST

START_TEST(round_bankers) {
  s21_decimal result;
  ck_assert_int_eq(s21_round(make_decimal(25, 1, 0), &result), 0);
  assert_bits(result, 2, 0, 0);
  ck_assert_int_eq(s21_round(make_decimal(35, 1, 0), &result), 0);
  assert_bits(result, 4, 0, 0);
  ck_assert_int_eq(s21_round(make_decimal(25, 1, 1), &result), 0);
  assert_bits(result, 2, 0, 1);
  ck_assert_int_eq(s21_round(make_decimal(15, 1, 1), &result), 0);
  assert_bits(result, 2, 0, 1);
  ck_assert_int_eq(s21_round(make_decimal(251, 2, 0), &result), 0);
  assert_bits(result, 3, 0, 0);
  ck_assert_int_eq(s21_round(make_decimal(249, 2, 0), &result), 0);
  assert_bits(result, 2, 0, 0);
  ck_assert_int_eq(s21_round(make_decimal(250, 2, 0), &result), 0);
  assert_bits(result, 2, 0, 0);
  ck_assert_int_eq(s21_round(make_decimal(8, 0, 0), &result), 0);
  assert_bits(result, 8, 0, 0);
  ck_assert_int_eq(s21_round(make_decimal(1, 0, 0), NULL), 1);
}
END_TEST

START_TEST(negate_flips_sign) {
  s21_decimal result;
  ck_assert_int_eq(s21_negate(make_decimal(5, 2, 0), &result), 0);
  assert_bits(result, 5, 2, 1);
  ck_assert_int_eq(s21_negate(result, &result), 0);
  assert_bits(result, 5, 2, 0);
  ck_assert_int_eq(s21_negate(make_decimal(0, 0, 0), &result), 0);
  assert_bits(result, 0, 0, 1);
  ck_assert_int_eq(s21_negate(make_decimal(1, 0, 0), NULL), 1);
}
END_TEST

Suite *s21_other_suite(void) {
  Suite *suite = suite_create("s21_other");
  TCase *core = tcase_create("core");
  tcase_add_test(core, truncate_drops_fraction);
  tcase_add_test(core, floor_toward_minus_inf);
  tcase_add_test(core, round_bankers);
  tcase_add_test(core, negate_flips_sign);
  suite_add_tcase(suite, core);
  return suite;
}
