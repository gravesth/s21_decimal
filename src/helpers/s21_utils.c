#include "s21_utils.h"

void s21_init_decimal(s21_decimal *decimal) {
  decimal->bits[0] = 0;
  decimal->bits[1] = 0;
  decimal->bits[2] = 0;
  decimal->bits[3] = 0;
}

int s21_get_sign(s21_decimal decimal) {
  return (int)(((uint32_t)decimal.bits[3] >> 31) & 1u);
}

void s21_set_sign(s21_decimal *decimal, int sign) {
  uint32_t bits = (uint32_t)decimal->bits[3];
  if (sign) {
    bits |= 0x80000000u;
  } else {
    bits &= ~0x80000000u;
  }
  decimal->bits[3] = (int)bits;
}

int s21_get_scale(s21_decimal decimal) {
  return (int)(((uint32_t)decimal.bits[3] >> 16) & 0xFFu);
}

void s21_set_scale(s21_decimal *decimal, int scale) {
  uint32_t bits = (uint32_t)decimal->bits[3];
  bits &= ~(0xFFu << 16);
  bits |= ((uint32_t)scale & 0xFFu) << 16;
  decimal->bits[3] = (int)bits;
}

int s21_is_zero(s21_decimal decimal) {
  return decimal.bits[0] == 0 && decimal.bits[1] == 0 && decimal.bits[2] == 0;
}

void s21_big_init(s21_big_decimal *value) {
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    value->bits[i] = 0;
  }
  value->scale = 0;
  value->sign = 0;
}

void s21_big_from_decimal(s21_decimal decimal, s21_big_decimal *value) {
  s21_big_init(value);
  value->bits[0] = (uint32_t)decimal.bits[0];
  value->bits[1] = (uint32_t)decimal.bits[1];
  value->bits[2] = (uint32_t)decimal.bits[2];
  value->scale = s21_get_scale(decimal);
  value->sign = s21_get_sign(decimal);
}

int s21_big_bitlen(const s21_big_decimal *value) {
  int length = 0;
  for (int word = S21_BIG_WORDS - 1; word >= 0 && length == 0; word--) {
    if (value->bits[word] != 0) {
      for (int bit = 31; bit >= 0; bit--) {
        if (((value->bits[word] >> bit) & 1u) != 0) {
          length = word * 32 + bit + 1;
          bit = -1;
        }
      }
    }
  }
  return length;
}

int s21_big_is_zero(const s21_big_decimal *value) {
  return s21_big_bitlen(value) == 0;
}

int s21_big_cmp_mag(const s21_big_decimal *left, const s21_big_decimal *right) {
  int result = 0;
  for (int i = S21_BIG_WORDS - 1; i >= 0 && result == 0; i--) {
    if (left->bits[i] > right->bits[i]) {
      result = 1;
    } else if (left->bits[i] < right->bits[i]) {
      result = -1;
    }
  }
  return result;
}

int s21_big_overflow96(const s21_big_decimal *value) {
  int overflow = 0;
  for (int i = 3; i < S21_BIG_WORDS; i++) {
    if (value->bits[i] != 0) overflow = 1;
  }
  return overflow;
}

int s21_big_mul10(s21_big_decimal *value) {
  uint64_t carry = 0;
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    uint64_t current = (uint64_t)value->bits[i] * 10ull + carry;
    value->bits[i] = (uint32_t)current;
    carry = current >> 32;
  }
  return carry != 0;
}

void s21_big_align(s21_big_decimal *left, s21_big_decimal *right) {
  while (left->scale < right->scale) {
    s21_big_mul10(left);
    left->scale++;
  }
  while (right->scale < left->scale) {
    s21_big_mul10(right);
    right->scale++;
  }
}

int s21_big_add(const s21_big_decimal *left, const s21_big_decimal *right,
                s21_big_decimal *result) {
  s21_big_decimal sum;
  s21_big_init(&sum);
  uint64_t carry = 0;
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    uint64_t current = (uint64_t)left->bits[i] + right->bits[i] + carry;
    sum.bits[i] = (uint32_t)current;
    carry = current >> 32;
  }
  int overflow = carry != 0;
  if (!overflow) *result = sum;
  return overflow;
}

void s21_big_sub(const s21_big_decimal *left, const s21_big_decimal *right,
                 s21_big_decimal *result) {
  s21_big_init(result);
  int borrow = 0;
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    int64_t current = (int64_t)left->bits[i] - (int64_t)right->bits[i] - borrow;
    if (current < 0) {
      current += (int64_t)1 << 32;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result->bits[i] = (uint32_t)current;
  }
}

void s21_big_mul(const s21_big_decimal *left, const s21_big_decimal *right,
                 s21_big_decimal *result) {
  s21_big_init(result);
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    uint64_t carry = 0;
    for (int j = 0; i + j < S21_BIG_WORDS; j++) {
      uint64_t current = (uint64_t)result->bits[i + j] +
                         (uint64_t)left->bits[i] * right->bits[j] + carry;
      result->bits[i + j] = (uint32_t)current;
      carry = current >> 32;
    }
  }
}

uint32_t s21_big_div10(s21_big_decimal *value) {
  uint64_t remainder = 0;
  for (int i = S21_BIG_WORDS - 1; i >= 0; i--) {
    uint64_t current = (remainder << 32) | value->bits[i];
    value->bits[i] = (uint32_t)(current / 10ull);
    remainder = current % 10ull;
  }
  return (uint32_t)remainder;
}

void s21_big_add_one(s21_big_decimal *value) {
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    value->bits[i]++;
    if (value->bits[i] != 0) i = S21_BIG_WORDS;
  }
}

void s21_big_shl1(s21_big_decimal *value) {
  uint32_t carry = 0;
  for (int i = 0; i < S21_BIG_WORDS; i++) {
    uint32_t next = value->bits[i] >> 31;
    value->bits[i] = (value->bits[i] << 1) | carry;
    carry = next;
  }
}

void s21_big_divmod(const s21_big_decimal *dividend,
                    const s21_big_decimal *divisor, s21_big_decimal *quotient,
                    s21_big_decimal *remainder) {
  s21_big_init(quotient);
  s21_big_init(remainder);
  int length = s21_big_bitlen(dividend);
  for (int i = length - 1; i >= 0; i--) {
    s21_big_shl1(remainder);
    uint32_t bit = (dividend->bits[i / 32] >> (i % 32)) & 1u;
    if (bit) remainder->bits[0] |= 1u;
    s21_big_shl1(quotient);
    if (s21_big_cmp_mag(remainder, divisor) >= 0) {
      s21_big_decimal difference;
      s21_big_sub(remainder, divisor, &difference);
      *remainder = difference;
      quotient->bits[0] |= 1u;
    }
  }
}

static int s21_should_round_up(int round_digit, int sticky,
                               const s21_big_decimal *number) {
  int round_up = 0;
  if (round_digit > 5) {
    round_up = 1;
  } else if (round_digit == 5 && (sticky || (number->bits[0] & 1u))) {
    round_up = 1;
  }
  return round_up;
}

int s21_big_to_decimal(const s21_big_decimal *value, int tail,
                       s21_decimal *result) {
  s21_big_decimal number = *value;
  int sticky = tail ? 1 : 0;
  int status = S21_OK;
  int fitted = 0;

  while (status == S21_OK && !fitted) {
    int round_digit = 0;
    while ((s21_big_overflow96(&number) || number.scale > 28) &&
           status == S21_OK) {
      if (number.scale == 0) {
        status = number.sign ? S21_TOO_SMALL : S21_TOO_BIG;
      } else {
        int remainder = (int)s21_big_div10(&number);
        number.scale--;
        if (round_digit != 0) sticky = 1;
        round_digit = remainder;
      }
    }
    if (status == S21_OK && s21_should_round_up(round_digit, sticky, &number)) {
      s21_big_add_one(&number);
      sticky = 0;
      if (!s21_big_overflow96(&number) && number.scale <= 28) fitted = 1;
    } else if (status == S21_OK) {
      fitted = 1;
    }
  }

  s21_init_decimal(result);
  if (status == S21_OK && !s21_big_is_zero(&number)) {
    result->bits[0] = (int)number.bits[0];
    result->bits[1] = (int)number.bits[1];
    result->bits[2] = (int)number.bits[2];
    s21_set_scale(result, number.scale);
    s21_set_sign(result, number.sign);
  }
  return status;
}
