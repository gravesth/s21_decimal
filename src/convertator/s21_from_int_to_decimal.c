#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (dst == NULL)
    return 1;
  init_decimal(dst);
  long long abs_src = src;
  if (src < 0){set_sign(dst, 1); abs_src = -abs_src;};
  dst->bits[0] = abs_src;

  return 0;
};