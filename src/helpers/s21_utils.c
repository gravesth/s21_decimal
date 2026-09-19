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
    b -> sign = (d.bit[3] >> 31) & 1;
    b -> scale = (d.bit[3] >> 16) & 0xFF;
    b -> bits[0] = d.bit[0];
    b -> bits[1] = d.bit[1];
    b -> bits[2] = d.bit[2];
    b -> bits[3] = 0;
    b -> bits[4] = 0;
    b -> bits[5] = 0;
}
void init_big_decimal(s21_big_decimal *d)
{
    for(int i = 0; i < 6; i++)
    {
        d->bits[i] = 0;
    }
    d->scale = 0;
    d->sign = 0;

}
void mul_by_10(s21_big_decimal *b)
{
    long long int carry = 0;
    for (int i = 0; i < 6; i++)
    {
        long long int current = (long long int)b->bits[i] * 10 + carry;

        b->bits[i] = current & 0xFFFFFFFF;

        carry = current >> 32;
    }

}
void big_normalize(s21_big_decimal *b_1, s21_big_decimal *b_2)
{
    while(b_1->scale > b_2->scale)
    {
        b_2->scale++;
        mul_by_10(b_2);
    }

    while(b_1->scale < b_2->scale)
    {
        b_1->scale++;
        mul_by_10(b_1);
    }
}
int div_by_10(s21_big_decimal *b)
{
    unsigned long long int carry = 0;
    for(int i = 5; i >= 0; i--)
    {
        
        long long int current = carry << 32 | b->bits[i];
        b->bits[i] = current/10;
        carry = current % 10;
    }
    return (int) carry;
}
void clean_zeroes(s21_big_decimal *b)
{
    s21_big_decimal current = *b;
    while(b->scale && div_by_10(&current) == 0)
    {
        div_by_10(b);
        b->scale--;
    }

}

void add_by_1(s21_big_decimal *b)
{
    int carry = 1;
    for(int i = 0; i < 6; i++)
    {
        unsigned long long int current = (unsigned long long int)b->bits[i] + carry;
        b->bits[i] = current & 0xFFFFFFFF;
        carry = current >> 32;
    }
}
void bankers_rounding(s21_big_decimal* b)
{
    clean_zeroes(b);
    int remainder = 0;
    while((b->bits[5] != 0 || b->bits[4] != 0 || b->bits[3] != 0 || b->scale > 28) && b->scale != 0)
    {
        remainder = div_by_10(b);
        b->scale--;
        
    }
    s21_big_decimal current = *b;
    int last_remainder = div_by_10(&current);
    if((remainder > 5) || (remainder == 5 && last_remainder%2)) add_by_1(b);
    clean_zeroes(b);
}

int get_decimal(s21_big_decimal b, s21_decimal *result)
{
    bankers_rounding(&b);
    int error = 0;
    if(b.bits[5] || b.bits[4] || b.bits[3])
    {
        if(b.sign) error = 2;
        else error = 1;
    }
    else{
        for(int i = 0; i < 3; i++) result->bit[i] = b.bits[i];
        set_scale(result, b.scale);
        set_sign(result, b.sign);
    }
    return error;
}

int get_bit_big_decimal(s21_big_decimal b, int index)
{
    int bit =  - 1;

    if(index >= 0 && index < 192)
    {
        bit = (b.bits[index/32] >> (index % 32)) & 1;
    }
    return bit;
}


int is_zero(s21_decimal d)
{
    int ans = 1;
    for(int i = 0; i <  96 && ans; i++)
    {
        if(get_bit(d, i)) ans = 0;
    }
    return ans;
}

s21_decimal abs_decimal(s21_decimal b)
{
    s21_decimal new_b = b;
    set_sign(&new_b, 0);
    return new_b;
}
