#ifndef S21_TEST_RUNNER_H_
#define S21_TEST_RUNNER_H_

#include <check.h>

Suite *s21_add_suite(void);
Suite *s21_sub_suite(void);
Suite *suite_comparison(void);
Suite *s21_mul_suite(void);
Suite *s21_div_suite(void);
Suite *s21_converters_suite(void);
Suite *s21_other_suite(void);
Suite *s21_extra_suite(void);

#endif
