#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"

int s21_negate(s21_decimal src, s21_decimal *dst){
    if(dst == NULL) return 1;

    *dst = src;

    if(get_sign(src) == 0) set_sign(dst, 1);
    else set_sign(dst, 0);

    return 0;
}