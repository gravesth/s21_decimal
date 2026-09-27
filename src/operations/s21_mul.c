#include "../s21_internal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) return S21_TOO_BIG;

  s21_big_decimal left;
  s21_big_decimal right;
  s21_big_from_decimal(value_1, &left);
  s21_big_from_decimal(value_2, &right);

  s21_big_decimal product;
  s21_big_mul(&left, &right, &product);
  product.sign = left.sign ^ right.sign;
  product.scale = left.scale + right.scale;
  return s21_big_to_decimal(&product, 0, result);
}
