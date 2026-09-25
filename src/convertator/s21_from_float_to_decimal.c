#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst || isnan(src) || isinf(src)) return 1;

  init_decimal(dst);

  if (fabsf(src) > 7.9228162514264337593543950335e28f) return 1;
  if (fabsf(src) > 0.0f && fabsf(src) < 1e-28f) return 1;

  if (fabsf(src) == 0.0f) {
    if (signbit(src)) set_sign(dst, 1);
    return 0;
  }

  char buf[64];
  sprintf(buf, "%.6e", fabsf(src));

  char *e_pos = strchr(buf, 'e');
  int exp = atoi(e_pos + 1);

  int mantissa = 0;
  for (char *p = buf; p < e_pos; p++) {
    if (*p >= '0' && *p <= '9') {
      mantissa = mantissa * 10 + (*p - '0');
    }
  }

  int scale = 6 - exp;

  while (scale > 28) {
    int rem = mantissa % 10;
    mantissa /= 10;
    if (rem > 5 || (rem == 5 && (mantissa % 2 != 0))) {
      mantissa++;
    }
    scale--;
  }

  while (scale > 0 && mantissa % 10 == 0) {
    mantissa /= 10;
    scale--;
  }

  s21_big_decimal b;
  init_big_decimal(&b);
  b.bits[0] = mantissa;

  while (scale < 0) {
    mul_by_10(&b);
    scale++;
  }

  b.scale = scale;
  b.sign = signbit(src) ? 1 : 0;

  int err = get_decimal(b, dst);
  return (err != 0) ? 1 : 0;
}
