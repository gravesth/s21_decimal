#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (result == NULL)
    return 1;
  int scale = get_scale(value);
  if (scale < 0 || scale > 28)
    return 1;

  s21_decimal truncated;
  int err = s21_truncate(value, &truncated);
  if (err)
    return err;

  if (get_sign(value) && !s21_is_equal(value, truncated)) {
    s21_decimal one = {{1, 0, 0, 0}};
    return s21_sub(truncated, one, result) ? 1 : 0;
  }

  *result = truncated;
  return 0;
}
