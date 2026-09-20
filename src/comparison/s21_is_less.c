#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2)
{
    int code = 0;

    s21_big_decimal big_value_1;
    s21_big_decimal big_value_2;

    get_big_decimal(value_1, &big_value_1);
    get_big_decimal(value_2, &big_value_2);

    big_normalize(&big_value_1, &big_value_2);


    if (is_zero(value_1) && is_zero(value_2))
    {
        code = 0;
    }
    else if(get_sign(value_1) == get_sign(value_2) && !get_sign(value_2))
    {
        for(int i = 191; i >= 0  && !code; i--)
        {
            if(get_bit_big_decimal(big_value_1, i) < get_bit_big_decimal(big_value_2, i)) code = 1;
            else if (get_bit_big_decimal(big_value_1, i) > get_bit_big_decimal(big_value_2, i)) break;

        }
    }
    else if(get_sign(value_1) == get_sign(value_2) && get_sign(value_2))
    {
        for(int i = 191; i >= 0  && !code; i--)
        {
            if(get_bit_big_decimal(big_value_1, i) > get_bit_big_decimal(big_value_2, i)) code = 1;
            else if (get_bit_big_decimal(big_value_1, i) < get_bit_big_decimal(big_value_2, i)) break;
        }
    }
    else
    {
        code = (int)(get_sign(value_1) > get_sign(value_2));
    }





    return code;
}