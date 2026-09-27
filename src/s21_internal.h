#ifndef S21_INTERNAL_H_
#define S21_INTERNAL_H_

#include <stddef.h>
#include <stdint.h>

#include "s21_decimal.h"

#define S21_BIG_WORDS 8
#define S21_OK 0
#define S21_TOO_BIG 1
#define S21_TOO_SMALL 2
#define S21_DIV_BY_ZERO 3
#define S21_CONVERT_ERROR 1

typedef struct {
  uint32_t bits[S21_BIG_WORDS];
  int scale;
  int sign;
} s21_big_decimal;

void s21_init_decimal(s21_decimal *decimal);
int s21_get_sign(s21_decimal decimal);
void s21_set_sign(s21_decimal *decimal, int sign);
int s21_get_scale(s21_decimal decimal);
void s21_set_scale(s21_decimal *decimal, int scale);
int s21_is_zero(s21_decimal decimal);

void s21_big_init(s21_big_decimal *value);
void s21_big_from_decimal(s21_decimal decimal, s21_big_decimal *value);
int s21_big_is_zero(const s21_big_decimal *value);
int s21_big_bitlen(const s21_big_decimal *value);
int s21_big_cmp_mag(const s21_big_decimal *left, const s21_big_decimal *right);
int s21_big_overflow96(const s21_big_decimal *value);
void s21_big_align(s21_big_decimal *left, s21_big_decimal *right);
int s21_big_add(const s21_big_decimal *left, const s21_big_decimal *right,
                s21_big_decimal *result);
void s21_big_sub(const s21_big_decimal *left, const s21_big_decimal *right,
                 s21_big_decimal *result);
void s21_big_mul(const s21_big_decimal *left, const s21_big_decimal *right,
                 s21_big_decimal *result);
int s21_big_mul10(s21_big_decimal *value);
uint32_t s21_big_div10(s21_big_decimal *value);
void s21_big_add_one(s21_big_decimal *value);
void s21_big_shl1(s21_big_decimal *value);
void s21_big_divmod(const s21_big_decimal *dividend,
                    const s21_big_decimal *divisor, s21_big_decimal *quotient,
                    s21_big_decimal *remainder);
int s21_big_to_decimal(const s21_big_decimal *value, int tail,
                       s21_decimal *result);

#endif
