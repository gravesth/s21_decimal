CC = gcc
CFLAGS =  -Wall -Werror -Wextra -std=c11

TARGET = s21_decimal.a

OBJ = operations/s21_add.o \
	  operations/s21_mul.o \
      operations/s21_sub.o \
      helpers/s21_utils.o \
      comparison/s21_is_equal.o \
      comparison/s21_is_less.o \
      comparison/s21_is_greater.o \
      comparison/s21_is_greater_or_equal.o \
      comparison/s21_is_less_or_equal.o \
      comparison/s21_is_not_equal.o \
	  operations/s21_div.o \
      converters/s21_from_int_to_decimal.o \
      converters/s21_from_float_to_decimal.o \
      converters/s21_from_decimal_to_int.o \
      converters/s21_from_decimal_to_float.o \
      other/s21_truncate.o \
      other/s21_negate.o \
      other/s21_floor.o \
      other/s21_round.o


CHECK_LIBS = -lcheck -lsubunit -lm -lpthread

GCOV_FLAGS = -fprofile-arcs -ftest-coverage

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)

operations/s21_div.o: operations/s21_div.c
	$(CC) $(CFLAGS) -c operations/s21_div.c -o operations/s21_div.o

operations/s21_mul.o: operations/s21_mul.c
	$(CC) $(CFLAGS) -c operations/s21_mul.c -o operations/s21_mul.o

operations/s21_add.o: operations/s21_add.c
	$(CC) $(CFLAGS) -c operations/s21_add.c -o operations/s21_add.o

operations/s21_sub.o: operations/s21_sub.c
	$(CC) $(CFLAGS) -c operations/s21_sub.c -o operations/s21_sub.o

helpers/s21_utils.o: helpers/s21_utils.c
	$(CC) $(CFLAGS) -c helpers/s21_utils.c -o helpers/s21_utils.o

comparison/s21_is_equal.o: comparison/s21_is_equal.c
	$(CC) $(CFLAGS) -c comparison/s21_is_equal.c -o comparison/s21_is_equal.o

comparison/s21_is_less.o: comparison/s21_is_less.c
	$(CC) $(CFLAGS) -c comparison/s21_is_less.c -o comparison/s21_is_less.o

comparison/s21_is_greater.o: comparison/s21_is_greater.c
	$(CC) $(CFLAGS) -c comparison/s21_is_greater.c -o comparison/s21_is_greater.o

comparison/s21_is_greater_or_equal.o: comparison/s21_is_greater_or_equal.c
	$(CC) $(CFLAGS) -c comparison/s21_is_greater_or_equal.c -o comparison/s21_is_greater_or_equal.o

comparison/s21_is_less_or_equal.o: comparison/s21_is_less_or_equal.c
	$(CC) $(CFLAGS) -c comparison/s21_is_less_or_equal.c -o comparison/s21_is_less_or_equal.o


comparison/s21_is_not_equal.o: comparison/s21_is_not_equal.c
	$(CC) $(CFLAGS) -c comparison/s21_is_not_equal.c -o comparison/s21_is_not_equal.o

converters/s21_from_int_to_decimal.o: converters/s21_from_int_to_decimal.c
	$(CC) $(CFLAGS) -c converters/s21_from_int_to_decimal.c -o converters/s21_from_int_to_decimal.o

converters/s21_from_float_to_decimal.o: converters/s21_from_float_to_decimal.c
	$(CC) $(CFLAGS) -c converters/s21_from_float_to_decimal.c -o converters/s21_from_float_to_decimal.o

converters/s21_from_decimal_to_int.o: converters/s21_from_decimal_to_int.c
	$(CC) $(CFLAGS) -c converters/s21_from_decimal_to_int.c -o converters/s21_from_decimal_to_int.o

converters/s21_from_decimal_to_float.o: converters/s21_from_decimal_to_float.c
	$(CC) $(CFLAGS) -c converters/s21_from_decimal_to_float.c -o converters/s21_from_decimal_to_float.o

other/s21_truncate.o: other/s21_truncate.c
	$(CC) $(CFLAGS) -c other/s21_truncate.c -o other/s21_truncate.o

other/s21_negate.o: other/s21_negate.c
	$(CC) $(CFLAGS) -c other/s21_negate.c -o other/s21_negate.o

other/s21_floor.o: other/s21_floor.c
	$(CC) $(CFLAGS) -c other/s21_floor.c -o other/s21_floor.o

other/s21_round.o: other/s21_round.c
	$(CC) $(CFLAGS) -c other/s21_round.c -o other/s21_round.o

test : clean $(TARGET)
	$(CC) $(CFLAGS) tests/test_runner.c tests/test_helpers.c tests/operations/*.c tests/converters/*.c tests/other/*.c tests/test_comparison.c $(TARGET) $(CHECK_LIBS) -o test
	./test


gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) tests/test_runner.c tests/test_helpers.c tests/operations/*.c tests/converters/*.c tests/other/*.c tests/test_comparison.c helpers/s21_utils.c comparison/*.c operations/*.c converters/*.c other/*.c $(CHECK_LIBS) -o test_gcov
	./test_gcov
	lcov --capture --directory . --base-directory . --output-file coverage.info --no-external --ignore-errors inconsistent
	genhtml coverage.info --output-directory report --ignore-errors source

style:
	clang-format -i operations/*.c comparison/*.c converters/*.c other/*.c *.c *.h tests/*.c tests/*.h tests/operations/*.c tests/converters/*.c tests/other/*.c

clean:
	rm -f $(TARGET) test test_gcov coverage.info
	rm -f operations/*.o helpers/*.o *.o comparison/*.o converters/*.o other/*.o
	rm -f operations/*.gcda operations/*.gcno comparison/*.gcda comparison/*.gcno helpers/*.gcda helpers/*.gcno converters/*.gcda converters/*.gcno other/*.gcda other/*.gcno *.gcda *.gcno
	rm -rf report


.PHONY: all test clean gcov_report style
