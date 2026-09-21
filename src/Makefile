CC = gcc
CFLAGS =  -Wall -Werror -Wextra -std=c11

TARGET = s21_decimal.a

OBJ = operations/s21_add.o \
      operations/s21_sub.o \
      helpers/s21_utils.o \
      comparison/s21_is_equal.o \
      comparison/s21_is_less.o \
      comparison/s21_is_greater.o \
      comparison/s21_is_greater_or_equal.o \
      comparison/s21_is_less_or_equal.o \
      comparison/s21_is_not_equal.o


CHECK_LIBS = -lcheck -lsubunit -lm -lpthread

GCOV_FLAGS = -fprofile-arcs -ftest-coverage

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)

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

test : clean $(TARGET)
	$(CC) $(CFLAGS) tests/test_runner.c tests/operations/*.c  tests/test_comparison.c $(TARGET)  $(CHECK_LIBS) -o test
	./test


gcov_report: clean
	$(CC) $(CFLAGS) $(GCOV_FLAGS) tests/test_runner.c tests/operations/*.c  tests/test_comparison.c  helpers/s21_utils.c comparison/*.c  operations/*.c $(CHECK_LIBS) -o test_gcov
	./test_gcov
	lcov --capture --directory . --base-directory . --output-file coverage.info --no-external --ignore-errors inconsistent
	genhtml coverage.info --output-directory report --ignore-errors source

style:
	clang-format -i operations/*.c comparison/*.c *.c *.h tests/*.c tests/*.h tests/operations/*.c

clean:
	rm -f $(TARGET) test test_gcov coverage.info
	rm -f operations/*.o helpers/*.o *.o comparison/*.o
	rm -f operations/*.gcda operations/*.gcno comparison/*.gcda comparison/*.gcno helpers/*.gcda helpers/*.gcno *.gcda *.gcno
	rm -rf report


.PHONY: all test clean gcov_report style
