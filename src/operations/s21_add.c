#include "../s21_internal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) return S21_TOO_BIG;

  s21_big_decimal left;
  s21_big_decimal right;
  s21_big_from_decimal(value_1, &left);
  s21_big_from_decimal(value_2, &right);
  s21_big_align(&left, &right);

  s21_big_decimal sum;
  s21_big_init(&sum);
  sum.scale = left.scale;
  if (left.sign == right.sign) {
    s21_big_add(&left, &right, &sum);
    sum.sign = left.sign;
    sum.scale = left.scale;
  } else if (s21_big_cmp_mag(&left, &right) >= 0) {
    s21_big_sub(&left, &right, &sum);
    sum.sign = left.sign;
    sum.scale = left.scale;
  } else {
    s21_big_sub(&right, &left, &sum);
    sum.sign = right.sign;
    sum.scale = left.scale;
  }
  return s21_big_to_decimal(&sum, 0, result);
}
