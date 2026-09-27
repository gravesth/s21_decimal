#include "../s21_internal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (result == NULL) return S21_TOO_BIG;
  s21_init_decimal(result);
  if (s21_is_zero(value_2)) return S21_DIV_BY_ZERO;
  if (s21_is_zero(value_1)) return S21_OK;

  s21_big_decimal dividend;
  s21_big_decimal divisor;
  s21_big_from_decimal(value_1, &dividend);
  s21_big_from_decimal(value_2, &divisor);
  int sign = dividend.sign ^ divisor.sign;

  while (dividend.scale < divisor.scale) {
    s21_big_mul10(&dividend);
    dividend.scale++;
  }

  s21_big_decimal quotient;
  s21_big_decimal remainder;
  s21_big_divmod(&dividend, &divisor, &quotient, &remainder);
  quotient.scale = dividend.scale - divisor.scale;
  quotient.sign = sign;

  int tail = 0;
  while (!s21_big_is_zero(&remainder) && quotient.scale <= 28 && !tail) {
    s21_big_decimal expanded = quotient;
    s21_big_decimal digit;
    s21_big_decimal next_remainder;
    s21_big_decimal next_quotient;
    int failed = s21_big_mul10(&expanded);
    if (!failed) {
      s21_big_mul10(&remainder);
      s21_big_divmod(&remainder, &divisor, &digit, &next_remainder);
      failed = s21_big_add(&expanded, &digit, &next_quotient);
    }
    if (failed) {
      tail = 1;
    } else {
      next_quotient.scale = quotient.scale + 1;
      next_quotient.sign = sign;
      quotient = next_quotient;
      remainder = next_remainder;
    }
  }
  if (!s21_big_is_zero(&remainder)) tail = 1;
  return s21_big_to_decimal(&quotient, tail, result);
}
