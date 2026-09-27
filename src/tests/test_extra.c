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

static void assert_same_bits(s21_decimal value, s21_decimal expected) {
  for (int i = 0; i < 4; i++) {
    ck_assert_uint_eq((uint32_t)value.bits[i], (uint32_t)expected.bits[i]);
  }
}

START_TEST(spec_bankers_subtraction) {
  s21_decimal max_value =
      make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal fraction = make_decimal(6, 0, 0, 1, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_sub(max_value, fraction, &result), 0);
  s21_decimal expected =
      make_decimal(0xFFFFFFFEu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  assert_same_bits(result, expected);
}
END_TEST

START_TEST(add_bankers_edges_and_null) {
  s21_decimal near_even =
      make_decimal(0xFFFFFFFEu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  s21_decimal half = make_decimal(5, 0, 0, 2, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_add(near_even, half, &result), 0);
  assert_same_bits(result, near_even);

  s21_decimal six = make_decimal(6, 0, 0, 2, 0);
  s21_decimal rounded =
      make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  ck_assert_int_eq(s21_add(near_even, six, &result), 0);
  assert_same_bits(result, rounded);

  s21_decimal max_scaled =
      make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  ck_assert_int_eq(s21_add(max_scaled, half, &result), 0);
  s21_decimal carried =
      make_decimal(0x9999999Au, 0x99999999u, 0x19999999u, 0, 0);
  assert_same_bits(result, carried);

  ck_assert_int_eq(s21_add(near_even, half, NULL), 1);
  ck_assert_int_eq(s21_sub(near_even, half, NULL), 1);
  ck_assert_int_eq(s21_mul(near_even, half, NULL), 1);
  ck_assert_int_eq(s21_div(near_even, half, NULL), 1);
}
END_TEST

START_TEST(signs_scales_and_zero) {
  s21_decimal left = make_decimal(15, 0, 0, 1, 1);
  s21_decimal right = make_decimal(150, 0, 0, 2, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_add(left, right, &result), 0);
  ck_assert_int_eq(s21_is_zero(result), 1);
  ck_assert_int_eq(s21_get_sign(result), 0);

  left = make_decimal(100, 0, 0, 0, 1);
  right = make_decimal(35, 0, 0, 1, 0);
  ck_assert_int_eq(s21_add(left, right, &result), 0);
  s21_decimal expected = make_decimal(965, 0, 0, 1, 1);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);

  left = make_decimal(0, 1, 0, 0, 0);
  right = make_decimal(1, 0, 0, 0, 0);
  ck_assert_int_eq(s21_sub(left, right, &result), 0);
  expected = make_decimal(0xFFFFFFFFu, 0, 0, 0, 0);
  assert_same_bits(result, expected);

  left = make_decimal(3, 0, 0, 0, 1);
  right = make_decimal(4, 0, 0, 0, 1);
  ck_assert_int_eq(s21_mul(left, right, &result), 0);
  expected = make_decimal(12, 0, 0, 0, 0);
  assert_same_bits(result, expected);
}
END_TEST

START_TEST(mul_div_scale_and_overflow) {
  s21_decimal tiny = make_decimal(1, 0, 0, 20, 0);
  s21_decimal result;
  ck_assert_int_eq(s21_mul(tiny, tiny, &result), 0);
  ck_assert_int_eq(s21_is_zero(result), 1);

  s21_decimal ten = make_decimal(10, 0, 0, 0, 0);
  s21_decimal four = make_decimal(4, 0, 0, 0, 0);
  ck_assert_int_eq(s21_div(ten, four, &result), 0);
  s21_decimal two_and_half = make_decimal(25, 0, 0, 1, 0);
  ck_assert_int_eq(s21_is_equal(result, two_and_half), 1);

  s21_decimal one = make_decimal(1, 0, 0, 0, 1);
  s21_decimal three = make_decimal(3, 0, 0, 0, 0);
  ck_assert_int_eq(s21_div(one, three, &result), 0);
  ck_assert_int_eq(s21_get_sign(result), 1);
  ck_assert_int_eq(s21_get_scale(result), 28);

  s21_decimal max_value =
      make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal divisor = make_decimal(11, 0, 0, 28, 0);
  ck_assert_int_eq(s21_div(max_value, divisor, &result), 1);

  divisor = make_decimal(11, 0, 0, 28, 1);
  ck_assert_int_eq(s21_div(max_value, divisor, &result), 2);

  s21_decimal half = make_decimal(5, 0, 0, 1, 0);
  ck_assert_int_eq(s21_add(max_value, half, &result), 1);
  s21_set_sign(&max_value, 1);
  ck_assert_int_eq(s21_sub(max_value, half, &result), 2);
}
END_TEST

START_TEST(comparison_scale_alignment) {
  s21_decimal max_value =
      make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0, 0);
  s21_decimal scaled =
      make_decimal(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 1, 0);
  ck_assert_int_eq(s21_is_greater(max_value, scaled), 1);
  ck_assert_int_eq(s21_is_less(scaled, max_value), 1);
  ck_assert_int_eq(
      s21_is_equal(make_decimal(15, 0, 0, 1, 0), make_decimal(150, 0, 0, 2, 0)),
      1);
  ck_assert_int_eq(
      s21_is_less(make_decimal(15, 0, 0, 1, 0), make_decimal(150, 0, 0, 2, 0)),
      0);
  s21_decimal neg_zero = make_decimal(0, 0, 0, 3, 1);
  s21_decimal pos_one = make_decimal(1, 0, 0, 0, 0);
  ck_assert_int_eq(s21_is_less(neg_zero, pos_one), 1);
  ck_assert_int_eq(s21_is_greater(pos_one, neg_zero), 1);
  ck_assert_int_eq(s21_is_greater(neg_zero, make_decimal(1, 0, 0, 0, 1)), 1);
}
END_TEST

Suite *s21_extra_suite(void) {
  Suite *suite = suite_create("s21_extra");
  TCase *core = tcase_create("core");
  tcase_add_test(core, spec_bankers_subtraction);
  tcase_add_test(core, add_bankers_edges_and_null);
  tcase_add_test(core, signs_scales_and_zero);
  tcase_add_test(core, mul_div_scale_and_overflow);
  tcase_add_test(core, comparison_scale_alignment);
  suite_add_tcase(suite, core);
  return suite;
}
