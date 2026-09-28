#include <math.h>
#include <stdio.h>

#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    init_decimal(dst);
    if (src == 0.0f) {
      if (signbit(src) != 0) {
        set_sign(dst, 1);
      }
    } else if (isnan(src) || isinf(src) || fabsf(src) < 1e-28f ||
               fabsf(src) >= 79228162514264337593543950336.0f) {
      error = 1;
    } else {
      char buffer[32];
      snprintf(buffer, sizeof(buffer), "%.6E", fabsf(src));
      unsigned int mantissa = (unsigned int)(buffer[0] - '0');
      for (int i = 2; i <= 7; i++) {
        mantissa = mantissa * 10U + (unsigned int)(buffer[i] - '0');
      }
      int exponent = 0;
      int exp_sign = buffer[9] == '-' ? -1 : 1;
      for (int i = 10; buffer[i] != '\0'; i++) {
        exponent = exponent * 10 + (buffer[i] - '0');
      }
      exponent *= exp_sign;

      s21_big_decimal big;
      init_big_decimal(&big);
      big.bits[0] = mantissa;
      big.sign = signbit(src) != 0 ? 1 : 0;
      if (exponent >= 6) {
        for (int i = 0; i < exponent - 6; i++) {
          mul_by_10(&big);
        }
        big.scale = 0;
      } else {
        big.scale = 6 - exponent;
      }
      error = get_decimal(big, dst);
      if (error != 0) {
        init_decimal(dst);
        error = 1;
      }
    }
  }
  return error;
}
