#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst){

    if(dst == NULL) return 1;

    if(get_scale(src) > 28) return 1;

    return 0;
}
