#include "s21_utils.h"

int get_bit(s21_decimal d, int index) {
    int bit = -1;

    if (index >= 0 && index < 128) {
        bit = 0;
        if ((d.bit[index / 32] >> (index % 32)) & 1) {
            bit = 1;
        }
    }

    return bit;
}

void set_bit(s21_decimal *d, int index, int value) {
    if (d != NULL && index >= 0 && index < 128 && (value == 0 || value == 1)) {
        if (value == 1) {
            d->bit[index / 32] |= (1U << (index % 32));
        } else {
            d->bit[index / 32] &= ~(1U << (index % 32));
        }
    }
}

int get_sign(s21_decimal d){
    int sign = 0;
    if (get_bit(d, 127) == 1) sign = 1;
    return sign;
}

void set_sign(s21_decimal *d, int value){
    if (d != NULL && (value == 0 || value == 1)){
        if (value == 1){
            d->bit[3] |= (1U << 31);
        } else {
            d->bit[3] &= ~(1U << 31);
        }
    }
}

int get_scale(s21_decimal d) {
    return (d.bit[3] >> 16) & 0xFF;
}

void set_scale(s21_decimal *d, int scale) {
    if (d != NULL && scale >= 0 && scale <= 28) {
        d->bit[3] &= ~(0xFF << 16);
        d->bit[3] |= (scale << 16);
    }
}

void init_decimal(s21_decimal *d){
    for (int i = 0; i < 4; i++){
        d->bit[i] = 0;
    }
}

void get_big_decimal(s21_decimal d, s21_big_decimal* b)
{
    b -> sign = d.bit[3] >> 31;
    b -> scale = (d.bit[3] >> 16) & 0xFF;
    b -> bits[0] = d.bit[0];
    b -> bits[1] = d.bit[1];
    b -> bits[2] = d.bit[2];
    d.bit[3] = 0;
    d.bit[4] = 0;
    d.bit[5] = 0;
}