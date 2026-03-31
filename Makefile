CC = gcc
CFLAGS = -Wall -Wextra -O2 -I./include -I./src/algorithms/Student1 -I./src/algorithms/Student2 -I./src/algorithms/Student3
LDFLAGS = -lm

LIB_SRCS = src/core/expression_api.c \
           src/algorithms/Student1/SingleStackCalculatorWithPostfixNotationConversion.c \
           src/algorithms/Student1/DoubleStackCalculatorWithoutExplicitConversionToPostfixNotation.c \
           src/algorithms/Student2/student2.c \
           src/algorithms/Student3/simplify.c \
           src/algorithms/Student3/differentiate.c \
           src/algorithms/Student3/expression_equal.c \
           src/algorithms/Student3/substitute.c

LIB_OBJS = $(LIB_SRCS:.c=.o)

all: benchmark

benchmark: $(LIB_OBJS) src/core/benchmark.c
	@$(CC) $(CFLAGS) $(LIB_OBJS) src/core/benchmark.c -o benchmark.exe $(LDFLAGS)
	@echo "✅ Benchmark ready"

run: benchmark
	@benchmark.exe

examples: $(LIB_OBJS)
	@echo "=== Running examples ==="
	@echo "--- Example: example1 ---"
	@$(CC) $(CFLAGS) examples/examples1.c $(LIB_OBJS) -o examples/examples1.exe $(LDFLAGS)
	@examples\examples1.exe
	@echo ""
	@echo "--- Example: example3 ---"
	@$(CC) $(CFLAGS) examples/examples3.c $(LIB_OBJS) -o examples/examples3.exe $(LDFLAGS)
	@examples\examples3.exe

tests: $(LIB_OBJS)
	@echo "=== Running tests ==="
	@echo ""
	@echo "--- Student 1 tests (tests/Student1) ---"
	@$(CC) $(CFLAGS) tests/Student1/Tests1.c $(LIB_OBJS) -o tests/Student1/Tests1.exe $(LDFLAGS)
	@tests\Student1\Tests1.exe
	@echo ""
	@echo "--- Student 3 tests (tests/Student3) ---"
	@$(CC) $(CFLAGS) tests/Student3/test_differentiate.c $(LIB_OBJS) -o tests/Student3/test_differentiate.exe $(LDFLAGS)
	@tests\Student3\test_differentiate.exe
	@$(CC) $(CFLAGS) tests/Student3/test_exp.c $(LIB_OBJS) -o tests/Student3/test_exp.exe $(LDFLAGS)
	@tests\Student3\test_exp.exe
	@$(CC) $(CFLAGS) tests/Student3/test_simplify.c $(LIB_OBJS) -o tests/Student3/test_simplify.exe $(LDFLAGS)
	@tests\Student3\test_simplify.exe
	@$(CC) $(CFLAGS) tests/Student3/test_substitute.c $(LIB_OBJS) -o tests/Student3/test_substitute.exe $(LDFLAGS)
	@tests\Student3\test_substitute.exe

all-run: benchmark examples tests
	@echo "✅ All runs completed"

clean:
	@del /Q src\core\*.o 2>nul
	@del /Q src\algorithms\Student1\*.o 2>nul
	@del /Q src\algorithms\Student2\*.o 2>nul
	@del /Q src\algorithms\Student3\*.o 2>nul
	@del /Q benchmark.exe 2>nul
	@del /Q examples\*.exe 2>nul
	@del /Q tests\Student1\*.exe 2>nul
	@del /Q tests\Student3\*.exe 2>nul
	@echo "✅ Cleanup completed"
