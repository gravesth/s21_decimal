#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    int scale = get_scale(src);
    if (scale > 28) {
      error = 1;
    } else {
      double value = (double)(unsigned int)src.bits[2];
      value = value * 4294967296.0 + (double)(unsigned int)src.bits[1];
      value = value * 4294967296.0 + (double)(unsigned int)src.bits[0];
      for (int i = 0; i < scale; i++) {
        value /= 10.0;
      }
      if (get_sign(src) == 1) {
        value = -value;
      }
      *dst = (float)value;
    }
  }
  return error;
}
