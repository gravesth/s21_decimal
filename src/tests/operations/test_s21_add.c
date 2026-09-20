#include "../../s21_decimal.h"
#include "../../helpers/s21_utils.h"

START_TEST(add_zero_test)
{
    s21_decimal d1, d2, result;

    init_decimal(&d1);
    init_decimal(&d2);
    init_decimal(&result);

    s21_add(d1, d2, &result);

    
}
END_TEST
