#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
    s21_big_decimal b_1, b_2, result_big;
    init_big_decimal(&result_big);
    get_big_decimal(value_1, &b_1);
    get_big_decimal(value_2, &b_2);
    big_normalize(&b_1, &b_2);
    int error = 0;
    if (!(b_1.sign || b_2.sign) || (b_1.sign && b_2.sign))
    {

        add_process(b_1, b_2, &result_big);
        result_big.sign = b_1.sign;
    }
    else
    {

        if (s21_is_less(abs_decimal(value_2), abs_decimal(value_1)))
        {
            sub_process(b_1, b_2, &result_big);
            result_big.sign = b_1.sign;
        }
        else
        {
            sub_process(b_2, b_1, &result_big);
            result_big.sign = b_2.sign;
        }
    }
    result_big.scale = b_1.scale;
    error = get_decimal(result_big, result);

    return error;
}