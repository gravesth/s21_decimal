#include "test_runner.h"

int main(void) {
  SRunner *runner = srunner_create(NULL);
  srunner_add_suite(runner, s21_add_suite());
  srunner_add_suite(runner, s21_sub_suite());
  srunner_add_suite(runner, suite_comparison());
  srunner_add_suite(runner, s21_mul_suite());
  srunner_add_suite(runner, s21_div_suite());
  srunner_add_suite(runner, s21_converters_suite());
  srunner_add_suite(runner, s21_other_suite());
  srunner_add_suite(runner, s21_extra_suite());
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return failed == 0 ? 0 : 1;
}
