CC=gcc
CFLAGS+=-Wall -Werror
INCLUDES=-I.

zelda.adventure: zelda.adventure.c room.c utils.c CuTest.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

run_tests: run_tests.c room.c utils.c CuTest.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

test: run_tests
	./run_tests

clean:
	@rm zelda.adventure run_tests
