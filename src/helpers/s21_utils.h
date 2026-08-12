#include "../s21_decimal.h"

int get_bit(s21_decimal d, int index); //получить любой бит по индексу (0 - 127)

void set_bit(s21_decimal *d, int index, int value); // (установить любой бит по индексу (0 - 127))

int get_sign(s21_decimal d); //получить знак (1 - отриц. ; 0 - полож.)

void set_sign(s21_decimal *d, int value); //установить знак 

int get_scale(s21_decimal d); //получить кол-во знаков после запятой 

void set_scale(s21_decimal *d, int scale); //установить кол-во знаков после запятой 

void init_decimal(s21_decimal *d); //обнулить decimal
