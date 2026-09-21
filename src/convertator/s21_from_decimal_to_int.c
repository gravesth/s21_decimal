#include"../s21_decimal.h"
#include"../helpers/s21_utils.h"


int s21_from_decimal_to_int(s21_decimal src, int *dst){
    if (dst == NULL)return 1;
    if(get_scale(src) > 28) return 1;

    s21_big_decimal d;
    init_big_decimal(&d);
    get_big_decimal(src, &d);

    while(d.scale > 0) {div_by_10(&d); d.scale--;}

    if(d.bits[1] != 0 || d.bits[2] != 0 || d.bits[3] != 0 
        || d.bits[4] != 0 || d.bits[5] != 0) return 1;

    if(d.sign == 0 && d.bits[0] > 2147483647U ) return 1;
    if(d.sign == 1 && d.bits[0] > 2147483648U ) return 1;

    if(d.sign == 0) *dst = (int)d.bits[0];
    else *dst = -(int)d.bits[0];

    return 0;
}