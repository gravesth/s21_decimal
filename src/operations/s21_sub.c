#include "../s21_internal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  uint32_t bits = (uint32_t)value_2.bits[3];
  bits ^= 0x80000000u;
  value_2.bits[3] = (int)bits;
  return s21_add(value_1, value_2, result);
}
