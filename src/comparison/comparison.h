#ifndef COMPARISON_H
#define COMPARISON_H

#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"


int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

#endif  // COMPARISON_H

