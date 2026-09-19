#include "../s21_decimal.h"

int get_bit(s21_decimal d, int index); //получить любой бит по индексу (0 - 127)

void set_bit(s21_decimal *d, int index, int value); // (установить любой бит по индексу (0 - 127))

int get_sign(s21_decimal d); //получить знак (1 - отриц. ; 0 - полож.)

void set_sign(s21_decimal *d, int value); //установить знак 

int get_scale(s21_decimal d); //получить кол-во знаков после запятой 

void set_scale(s21_decimal *d, int scale); //установить кол-во знаков после запятой 

void init_decimal(s21_decimal *d); //обнулить decimal

void get_big_decimal(s21_decimal d, s21_big_decimal* b); //переход к расширенной структуре

void init_big_decimal(s21_big_decimal *d); //инициализация расширенной структуры

void big_normalize(s21_big_decimal *b_1, s21_big_decimal *b_2); //привод к общему масштабу

void mul_by_10(s21_big_decimal *b); //умножение на 10 метиссы расширенной структуры (вспомогтельная функция для big_normalize)

int div_by_10(s21_big_decimal *b); //деление на 10 расширенной структуры  (для get decimal)

void clean_zeroes(s21_big_decimal *b); // удаление незначащих нулей   (для get decimal)

void add_by_1(s21_big_decimal *b); //прибавление 1 к метиссе  (для get decimal)

void bankers_rounding(s21_big_decimal* b); //банковское округление (для get decimal)

int get_decimal(s21_big_decimal b, s21_decimal *result); // переход к стандартному децималу


int get_bit_big_decimal(s21_big_decimal b, int index); //получение бита из big decimal

int is_zero(s21_decimal d); //проверка decimal на +-0

s21_decimal abs_decimal(s21_decimal b); //децимал по модулю (для сравнения по модулю)