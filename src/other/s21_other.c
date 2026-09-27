#include "../s21_internal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return S21_CONVERT_ERROR;

  s21_big_decimal number;
  s21_big_from_decimal(value, &number);
  while (number.scale > 0) {
    s21_big_div10(&number);
    number.scale--;
  }
  number.scale = 0;
  return s21_big_to_decimal(&number, 0, result);
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return S21_CONVERT_ERROR;

  s21_decimal truncated;
  s21_truncate(value, &truncated);
  int status = S21_OK;
  if (s21_get_sign(value) && !s21_is_equal(value, truncated)) {
    s21_decimal one;
    s21_init_decimal(&one);
    one.bits[0] = 1;
    status = s21_sub(truncated, one, result);
  } else {
    *result = truncated;
  }
  return status;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return S21_CONVERT_ERROR;

  s21_big_decimal number;
  s21_big_from_decimal(value, &number);
  int round_digit = 0;
  int sticky = 0;
  while (number.scale > 0) {
    int remainder = (int)s21_big_div10(&number);
    number.scale--;
    if (number.scale == 0) {
      round_digit = remainder;
    } else if (remainder != 0) {
      sticky = 1;
    }
  }
  if (round_digit > 5 ||
      (round_digit == 5 && (sticky || (number.bits[0] & 1u)))) {
    s21_big_add_one(&number);
  }
  number.scale = 0;
  return s21_big_to_decimal(&number, 0, result);
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return S21_CONVERT_ERROR;
  *result = value;
  uint32_t bits = (uint32_t)result->bits[3];
  bits ^= 0x80000000u;
  result->bits[3] = (int)bits;
  return S21_OK;
}
