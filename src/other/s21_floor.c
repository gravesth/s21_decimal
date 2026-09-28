#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL) {
    error = 1;
  } else {
    int scale = get_scale(value);
    if (scale > 28) {
      init_decimal(result);
      error = 1;
    } else {
      error = s21_truncate(value, result);
      if (error == 0 && get_sign(value) == 1) {
        int fraction = 0;
        s21_big_decimal big;
        get_big_decimal(value, &big);
        for (int i = 0; i < scale && fraction == 0; i++) {
          if (div_by_10(&big) != 0) {
            fraction = 1;
          }
        }
        if (fraction == 1) {
          s21_decimal one;
          init_decimal(&one);
          one.bits[0] = 1;
          error = s21_sub(*result, one, result);
        }
      }
    }
  }
  return error;
}
