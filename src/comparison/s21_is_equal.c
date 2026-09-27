#include "../s21_internal.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int equal = 0;
  if (s21_is_zero(value_1) && s21_is_zero(value_2)) {
    equal = 1;
  } else if (s21_get_sign(value_1) == s21_get_sign(value_2)) {
    s21_big_decimal left;
    s21_big_decimal right;
    s21_big_from_decimal(value_1, &left);
    s21_big_from_decimal(value_2, &right);
    s21_big_align(&left, &right);
    equal = s21_big_cmp_mag(&left, &right) == 0;
  }
  return equal;
}
