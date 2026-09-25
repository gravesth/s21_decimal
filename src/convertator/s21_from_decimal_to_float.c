#include <math.h>

#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (dst == NULL) return 1;

  int scale = get_scale(src);
  if (scale > 28 || scale < 0) return 1;

  double temp = 0.0;
  for (int i = 0; i < 96; i++) {
    if (get_bit(src, i)) {
      temp += pow(2.0, i);
    }
  }

  while (scale > 0) {
    temp /= 10.0;
    scale--;
  }

  if (get_sign(src)) {
    temp = -temp;
  }

  *dst = (float)temp;
  return 0;
}
