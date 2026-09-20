#include "../s21_decimal.h"
#include "../helpers/s21_utils.h"

int s21_is_equal(s21_decimal value1, s21_decimal value2)
{

    int eq = 1;
    if (!(is_zero(value1) && is_zero(value2)))
    {
        s21_big_decimal b1, b2;

        get_big_decimal(value1, &b1);
        get_big_decimal(value2, &b2);

        big_normalize(&b1, &b2);
        if (b1.sign != b2.sign)
            eq = 0;
        for (int i = 0; i < 6 && eq; i++)
        {
            eq = (b1.bits[i] == b2.bits[i]);
        }
    }
    return eq;
}