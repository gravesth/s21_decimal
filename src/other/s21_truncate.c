#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL) {
    error = 1;
  } else {
    int scale = get_scale(value);
    if (scale > 28) {
      init_decimal(result);
      error = 1;
    } else {
      s21_big_decimal big;
      get_big_decimal(value, &big);
      for (int i = 0; i < scale; i++) {
        div_by_10(&big);
      }
      init_decimal(result);
      result->bits[0] = (int)big.bits[0];
      result->bits[1] = (int)big.bits[1];
      result->bits[2] = (int)big.bits[2];
      if (big.bits[0] != 0 || big.bits[1] != 0 || big.bits[2] != 0) {
        set_sign(result, get_sign(value));
      }
    }
  }
  return error;
}
