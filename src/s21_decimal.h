#include <stdio.h>
#include <check.h>

/// основная структура(массив int из 4 чисел, где первые 3 ячейки типа int по 32 бита каждая - основное число; последняя ячейка - служебная)
typedef struct
{
    int bit[4];
} s21_decimal;

/// расширенная структура для промежуточных вычислений
typedef struct
{
    unsigned int bits[6]; // 6 элементов по 32 бита = 192 бита мантиссы
    int scale;            // Масштаб (0..56 в промежуточных вычислениях)
    int sign;             // Знак (0 или 1)
} s21_big_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_is_equal(s21_decimal value1, s21_decimal value2);