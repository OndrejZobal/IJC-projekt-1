# Makefile
# Řešení IJC-DU1, příklad a), 20.3.2022
# Autor: Ondřej Zobal, FIT

CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra
LDFLAGS = -Wall -Wextra
LIBS = -lm
STEGFILE = du1-obrazek.ppm


# Adds either debug info or optimization
ifeq ($(debug), 1)
CFLAGS += -g
LDFLAGS += -g
else
CFLAGS += -O2
endif

# Compile to 32bit
ifeq ($(32bit), 1)
CFLAGS += -m32
LDFLAGS += -m32
endif


.PHONY: all run clean primes-i-full

# Builds both variants of the program
all: primes primes-i steg-decode

# Builds and runs both variants of the program
run: all
	./primes
	./primes-i

# Removes objects and executable files.
clean:
	find . -maxdepth 1 -type f -executable -exec rm {} +
	rm *.o

### A)

# MAKROS
primes: primes.o eratosthenes.o error.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# primes.o
primes.o: primes.c eratosthenes.h
	$(CC) $(CFLAGS) -c -o $@ primes.c

# eratosthenes.o
eratosthenes.o: eratosthenes.c bitset.h
	$(CC) $(CFLAGS) -c -o $@ eratosthenes.c

# INLINES
# Calls another instance of make and builds primes with inline functions
primes-i: primes-i.o eratosthenes-i.o error.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

# primes-i.o
primes-i.o: primes.c eratosthenes.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c -o $@ primes.c

# eratosthenes.o
eratosthenes-i.o: eratosthenes.c bitset.h
	$(CC) $(CFLAGS) -DUSE_INLINE -c -o $@ eratosthenes.c

### B)

error.o: error.c error.h
	$(CC) $(CFLAGS) -c -o $@ error.c

ppm.o: ppm.c
	$(CC) $(CFLAGS) -c -o $@ $^

steg-decode.o: steg-decode.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Linking steg-decode
steg-decode: ppm.o eratosthenes.o error.o steg-decode.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
