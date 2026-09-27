CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
CHECK_CFLAGS = $(shell pkg-config --cflags check)
CHECK_LIBS = $(shell pkg-config --libs check) -pthread

SRC = helpers/s21_utils.c \
      operations/s21_add.c \
      operations/s21_sub.c \
      operations/s21_mul.c \
      operations/s21_div.c \
      comparison/s21_is_equal.c \
      comparison/s21_is_not_equal.c \
      comparison/s21_is_less.c \
      comparison/s21_is_less_or_equal.c \
      comparison/s21_is_greater.c \
      comparison/s21_is_greater_or_equal.c \
      converters/s21_converters.c \
      other/s21_other.c

OBJ = $(SRC:.c=.o)

TEST_SRC = tests/test_runner.c \
           tests/test_comparison.c \
           tests/test_converters.c \
           tests/test_other.c \
           tests/test_extra.c \
           tests/operations/test_s21_add.c \
           tests/operations/test_s21_sub.c \
           tests/operations/test_s21_mul.c \
           tests/operations/test_s21_div.c

.PHONY: all clean test gcov_report s21_decimal.a

all: s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rcs $@ $(OBJ)

%.o: %.c s21_decimal.h s21_internal.h
	$(CC) $(CFLAGS) -c $< -o $@

test: s21_decimal.a
	$(CC) $(CFLAGS) $(CHECK_CFLAGS) $(TEST_SRC) s21_decimal.a $(CHECK_LIBS) -o test
	./test

gcov_report:
	$(MAKE) clean
	$(MAKE) test CFLAGS="$(CFLAGS) --coverage" CHECK_LIBS="$(CHECK_LIBS) --coverage"
	lcov --capture --directory . --output-file coverage.info --no-external
	lcov --remove coverage.info '*/tests/*' --output-file coverage.info
	genhtml coverage.info --output-directory report

clean:
	rm -rf $(OBJ) s21_decimal.a test coverage.info report
	find . \( -name '*.gcda' -o -name '*.gcno' -o -name '*.gcov' \) -delete
