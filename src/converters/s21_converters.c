#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_internal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL) return S21_CONVERT_ERROR;
  s21_init_decimal(dst);
  if (src < 0) {
    s21_set_sign(dst, 1);
    dst->bits[0] = (int)((uint32_t)(-(src + 1)) + 1u);
  } else {
    dst->bits[0] = src;
  }
  return S21_OK;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (dst == NULL) return S21_CONVERT_ERROR;

  s21_decimal truncated;
  s21_truncate(src, &truncated);
  if (truncated.bits[1] != 0 || truncated.bits[2] != 0)
    return S21_CONVERT_ERROR;

  uint32_t magnitude = (uint32_t)truncated.bits[0];
  int status = S21_OK;
  if (s21_get_sign(truncated)) {
    if (magnitude > 2147483648u) {
      status = S21_CONVERT_ERROR;
    } else if (magnitude == 2147483648u) {
      *dst = -2147483647 - 1;
    } else {
      *dst = -(int)magnitude;
    }
  } else if (magnitude > 2147483647u) {
    status = S21_CONVERT_ERROR;
  } else {
    *dst = (int)magnitude;
  }
  return status;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) return S21_CONVERT_ERROR;

  s21_big_decimal number;
  s21_big_from_decimal(src, &number);
  if (s21_big_is_zero(&number)) {
    *dst = number.sign ? -0.0f : 0.0f;
    return S21_OK;
  }

  char digits[40];
  int count = 0;
  while (!s21_big_is_zero(&number) && count < 40) {
    digits[count] = (char)('0' + s21_big_div10(&number));
    count++;
  }

  char buffer[64];
  int pos = 0;
  if (s21_get_sign(src)) buffer[pos++] = '-';
  buffer[pos++] = digits[count - 1];
  if (count > 1) {
    buffer[pos++] = '.';
    for (int i = count - 2; i >= 0; i--) buffer[pos++] = digits[i];
  }
  int exponent = (count - 1) - s21_get_scale(src);
  snprintf(buffer + pos, sizeof(buffer) - (size_t)pos, "e%d", exponent);
  *dst = strtof(buffer, NULL);
  return S21_OK;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (dst == NULL) return S21_CONVERT_ERROR;
  s21_init_decimal(dst);
  if (isnan(src) || isinf(src)) return S21_CONVERT_ERROR;

  int sign = signbit(src) ? 1 : 0;
  if (src == 0.0f) {
    if (sign) s21_set_sign(dst, 1);
    return S21_OK;
  }

  double absolute = fabs((double)src);
  if (absolute < 1e-28 || absolute >= 0x1p96) return S21_CONVERT_ERROR;

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%.6e", absolute);

  uint32_t mantissa = (uint32_t)(buffer[0] - '0');
  for (int i = 2; i < 8; i++) {
    mantissa = mantissa * 10u + (uint32_t)(buffer[i] - '0');
  }
  int index = 9;
  int exp_sign = 1;
  if (buffer[index] == '+' || buffer[index] == '-') {
    if (buffer[index] == '-') exp_sign = -1;
    index++;
  }
  int exponent = 0;
  while (buffer[index] >= '0' && buffer[index] <= '9') {
    exponent = exponent * 10 + (buffer[index] - '0');
    index++;
  }
  exponent *= exp_sign;

  s21_big_decimal number;
  s21_big_init(&number);
  number.bits[0] = mantissa;
  number.sign = sign;
  int scale = 6 - exponent;
  if (scale < 0) {
    for (int i = 0; i < -scale; i++) s21_big_mul10(&number);
    scale = 0;
  }
  number.scale = scale;
  if (s21_big_to_decimal(&number, 0, dst) != S21_OK) return S21_CONVERT_ERROR;
  if (s21_is_zero(*dst)) return S21_CONVERT_ERROR;
  return S21_OK;
}
