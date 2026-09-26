#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
    s21_big_decimal b_1, b_2, result_big;
    init_big_decimal(&result_big);
    get_big_decimal(value_1, &b_1);
    get_big_decimal(value_2, &b_2);
    s21_big_decimal remainder, digit;

    int error = div_process(b_1, b_2, &remainder, &result_big);
    int scale_raw = b_1.scale - b_2.scale;
    int len = len_big_decimal(result_big);



    if (error != 3 && len_big_decimal(result_big) != 0)
    {
        while (len <= 188 && len_big_decimal(remainder) != 0 && scale_raw <= 30)
        {
            mul_by_10(&remainder);
            div_process(remainder, b_2, &remainder, &digit);
            mul_by_10(&result_big);
            add_process(result_big, digit, &result_big);

            len = len_big_decimal(result_big);
            scale_raw++;
        }
        if (scale_raw < 0)
        {
            scale_raw *= -1;
            while (scale_raw > 0 && len <= 188)
            {
                mul_by_10(&result_big);
                len = len_big_decimal(result_big);
                scale_raw--;
            }
        }
        else
        {
            result_big.scale = scale_raw;
        }
        result_big.sign = (b_1.sign != b_2.sign);
        error = get_decimal(result_big, result);
    }
    return error;
}