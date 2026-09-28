#include <limits.h>

#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    init_decimal(dst);
    if (src < 0) {
      set_sign(dst, 1);
      if (src == INT_MIN) {
        dst->bits[0] = (int)2147483648U;
      } else {
        dst->bits[0] = -src;
      }
    } else {
      dst->bits[0] = src;
    }
  }
  return error;
}
