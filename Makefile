CC=gcc
CC_ARGS=-g -std=c11 -pedantic -Wall -Wextra

test: test.c bitset.h bitset.c
	${CC} ${CC_ARGS} test.c -o build/test

run: build/test
	build/test

primes: eratosthenes.c
	${CC} ${CC_ARGS} eratosthenes.c -o build/primes -lm
	build/primes
