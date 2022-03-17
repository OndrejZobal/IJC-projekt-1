# Makefile
# Řešení IJC-DU1, příklad a), 20.3.2022
# Autor: Ondřej Zobal, FIT

CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra
SUFFIX =
LIBS = -lm
STEGFILE = du1-obrazek.ppm


# Adds either debug info or optimization
ifeq ($(debug), 1)
CFLAGS += -g
else
CFLAGS += -O2
endif

# Will build the inline variant
ifeq ($(USE_INLINE), 1)
CFLAGS += -DUSE_INLINE
SUFFIX += -i
endif

# Compile to 32bit
ifeq ($(32bit), 1)
CFLAGS += -m32
endif


.PHONY: all run clean primes-i-full

# Builds both variants of the program
all: primes primes-i steg-decode

# Builds and runs both variants of the program
run: build
	./primes
	./primes-i
	./steg-decode $(STEGFILE)

# Removes objects and executable files.
clean:
	find . -maxdepth 1 -type f -executable -exec rm {} +
	rm *.o

# Calls another instance of make and builds primes with inline functions
primes-i: primes$(SUFFIX).o eratosthenes$(SUFFIX).o error.o
	make debug=$(debug) 32bit=$(32bit) USE_INLINE=1 primes-i-full

# primes (exec)
primes-i-full: primes$(SUFFIX).o eratosthenes$(SUFFIX).o error.o
	$(CC) $(CFLAGS) -o primes-i $^ $(LIBS)

primes: primes$(SUFFIX).o eratosthenes$(SUFFIX).o error.o
	$(CC) $(CFLAGS) -o primes $^ $(LIBS)

error.o: error.c error.h
	$(CC) $(CFLAGS) -c -o $@ error.c

# primes.o
primes$(SUFFIX).o: primes.c eratosthenes.h
	$(CC) $(CFLAGS) -c -o $@ primes.c

# eratosthenes.o
eratosthenes$(SUFFIX).o: eratosthenes.c bitset.h
	$(CC) $(CFLAGS) -c -o $@ eratosthenes.c

ppm.o: ppm.c
	$(CC) $(CFLAGS) -c -o $@ $^

steg-decode.o: steg-decode.c
	$(CC) $(CFLAGS) -c -o $@ $^

steg-decode: ppm.o eratosthenes$(SUFFIX).o error.o steg-decode.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
