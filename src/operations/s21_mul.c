#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"


int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
    s21_big_decimal b_1, b_2, result_big;
    init_big_decimal(&result_big);
    get_big_decimal(value_1, &b_1);
    get_big_decimal(value_2, &b_2);
    int error = 0;

    for(int i = 0; i < 96; i++)
    {
        int x = (b_2.bits[i/32] >> (i % 32)) & 1;
        if(x) 
        {
            add_process(result_big, big_shl(b_1, i), &result_big);
        }
    }
    result_big.sign = (b_1.sign != b_2.sign);
    result_big.scale = b_1.scale + b_2.scale;
    error = get_decimal(result_big, result);
    return error;
}