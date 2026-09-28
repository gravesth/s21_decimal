#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  if (result == NULL) {
    error = 1;
  } else {
    *result = value;
    set_sign(result, get_sign(value) == 0 ? 1 : 0);
  }
  return error;
}
