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
    if (get_bit(d, 127) == 1) return 1;
    return 0;
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
