#include <math.h>
#include"../s21_decimal.h"
#include "../helpers/s21_utils.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (!dst || isnan(src) || isinf(src)) return 1;
  if (fabsf(src) > 7.9228162514264337593543950335e28f) return 1;
  if (fabsf(src) > 0.0f && fabsf(src) < 1e-28f) return 1;

  init_decimal(dst);
  if (fabsf(src) == 0.0f) {
    if (signbit(src)) set_sign(dst, 1);
    return 0;
  }

  char buf[64];
  int int_part = 0, frac_part = 0, exp = 0;
  sprintf(buf, "%.6e", fabsf(src));
  sscanf(buf, "%d.%de%d", &int_part, &frac_part, &exp);

  s21_big_decimal b;
  init_big_decimal(&b);
  b.bits[0] = int_part * 1000000 + frac_part;

  int scale = 6 - exp;
  while (scale < 0) {
    mul_by_10(&b);
    scale++;
  }
  while (scale > 0 && b.bits[0] % 10 == 0) {
    div_by_10(&b);
    scale--;
  }

  int err = get_decimal(b, dst);
  if (!err) {
    set_scale(dst, scale);
    if (signbit(src)) set_sign(dst, 1);
  }
  return err;
}
