#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL)
    return 1;
  int scale = get_scale(value);
  if (scale < 0 || scale > 28)
    return 1;

  int sign = get_sign(value);
  s21_decimal value_unsigned = abs_decimal(value);
  s21_decimal integral;
  int err = s21_truncate(value_unsigned, &integral);
  if (err)
    return err;

  s21_decimal fractional;
  s21_sub(value_unsigned, integral, &fractional);

  s21_decimal point_five = {{5, 0, 0, 0}};
  set_scale(&point_five, 1);

  if (s21_is_equal(fractional, point_five)) {
    if ((integral.bits[0] & 1) != 0) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_add(integral, one, &integral);
    }
  } else if (s21_is_greater(fractional, point_five)) {
    s21_decimal one = {{1, 0, 0, 0}};
    s21_add(integral, one, &integral);
  }

  *result = integral;
  set_sign(result, sign);
  return 0;
}
