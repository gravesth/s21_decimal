#include <limits.h>

#include "../helpers/s21_utils.h"
#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;
  if (dst == NULL) {
    error = 1;
  } else {
    s21_decimal truncated;
    error = s21_truncate(src, &truncated);
    if (error == 0) {
      unsigned int magnitude = (unsigned int)truncated.bits[0];
      if (truncated.bits[1] != 0 || truncated.bits[2] != 0) {
        error = 1;
      } else if (get_sign(truncated) == 1) {
        if (magnitude > 2147483648U) {
          error = 1;
        } else if (magnitude == 2147483648U) {
          *dst = INT_MIN;
        } else {
          *dst = -(int)magnitude;
        }
      } else if (magnitude > 2147483647U) {
        error = 1;
      } else {
        *dst = (int)magnitude;
      }
    }
  }
  return error;
}
