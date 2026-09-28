#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (result == NULL)
    return 1;
  int scale = get_scale(value);
  if (scale < 0 || scale > 28)
    return 1;

  init_decimal(result);
  s21_big_decimal b;
  init_big_decimal(&b);
  get_big_decimal(value, &b);

  while (b.scale > 0) {
    div_by_10(&b);
    b.scale--;
  }

  for (int i = 0; i < 3; i++) {
    result->bits[i] = b.bits[i];
  }
  set_scale(result, 0);
  set_sign(result, b.sign);

  return 0;
}
