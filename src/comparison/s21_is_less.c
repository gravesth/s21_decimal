#include "../s21_internal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int less = 0;
  if (!(s21_is_zero(value_1) && s21_is_zero(value_2))) {
    int sign_1 = s21_get_sign(value_1);
    int sign_2 = s21_get_sign(value_2);
    if (sign_1 != sign_2) {
      less = sign_1 > sign_2;
    } else {
      s21_big_decimal left;
      s21_big_decimal right;
      s21_big_from_decimal(value_1, &left);
      s21_big_from_decimal(value_2, &right);
      s21_big_align(&left, &right);
      int cmp = s21_big_cmp_mag(&left, &right);
      less = sign_1 ? cmp > 0 : cmp < 0;
    }
  }
  return less;
}
