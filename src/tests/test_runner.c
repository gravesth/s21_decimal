#include "test_runner.h"

int main() {
  SRunner *sr = srunner_create(NULL);
  srunner_add_suite(sr, s21_add_suite());
  srunner_add_suite(sr, s21_sub_suite());
  srunner_add_suite(sr, suite_comparison());
  srunner_add_suite(sr, s21_mul_suite());
  srunner_add_suite(sr, s21_div_suite());
  srunner_add_suite(sr, s21_from_int_to_decimal_suite());
  srunner_add_suite(sr, s21_from_float_to_decimal_suite());
  srunner_add_suite(sr, s21_from_decimal_to_int_suite());
  srunner_add_suite(sr, s21_from_decimal_to_float_suite());
  srunner_add_suite(sr, s21_truncate_suite());
  srunner_add_suite(sr, s21_negate_suite());
  srunner_add_suite(sr, s21_floor_suite());
  srunner_add_suite(sr, s21_round_suite());
  srunner_add_suite(sr, s21_helpers_suite());
  srunner_run_all(sr, CK_NORMAL);

  int failed = srunner_ntests_failed(sr);

  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}