#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result)
{
    s21_big_decimal b_1, b_2, result_big;
    init_big_decimal(&result_big);
    get_big_decimal(value_1, &b_1);
    get_big_decimal(value_2, &b_2);
    big_normalize(&b_1, &b_2);
    int carry = 0;
    int error = 0;
    if(!(b_1.sign || b_2.sign) || (b_1.sign && b_2.sign))
    {
        for(int i = 0; i < 192; i++)
        {
            int x = b_1.bits[i/32] >> (i%32) & 1;
            int y = b_2.bits[i/32] >> (i%32) & 1;
            int result_d = x + y + carry;
            if(result_d < 2)
            {
                result_big.bits[i/32] |= (result_d << (i % 32));
                carry = 0;
            }
            else{
                result_big.bits[i/32] |= ((result_d - 2 ) << (i % 32));
                carry = 1;
            }
        }
        result_big.sign = b_1.sign;
        result_big.scale = b_1.scale;
        error = get_decimal(result_big, result);  
    }
    return error;
} 