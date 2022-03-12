CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra
BUILD_DIR = .
SUFFIX =
LIBS = -lm


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

# Will build the inline variant
ifeq ($(32bit), 1)
CFLAGS += -m32
endif


.PHONY: build run clean primes-i-full

# Builds both variants of the program
build: primes primes-i

# Builds and runs both variants of the program
run: build
	$(BUILD_DIR)/primes
	$(BUILD_DIR)/primes-i

# Removes objects and executable files from BUILD_DIR.
clean:
	find $(BUILD_DIR) -maxdepth 1 -type f -executable -exec rm {} +
	rm $(BUILD_DIR)/*.o

# Calls another instance of make and builds primes with inline functions
primes-i:
	make debug=$(debug) USE_INLINE=1 primes-i-full

# primes (exec), calls primes.o, eratosthenes.o and bitset.o
primes-i-full: $(BUILD_DIR)/primes$(SUFFIX).o $(BUILD_DIR)/eratosthenes$(SUFFIX).o
	$(CC) $(CFLAGS) -o primes-i $^ $(LIBS)

primes: $(BUILD_DIR)/primes$(SUFFIX).o $(BUILD_DIR)/eratosthenes$(SUFFIX).o
	$(CC) $(CFLAGS) -o primes $^ $(LIBS)

# primes.o
$(BUILD_DIR)/primes$(SUFFIX).o: primes.c
	$(CC) $(CFLAGS) -c -o $@ $^

# eratosthenes.o
$(BUILD_DIR)/eratosthenes$(SUFFIX).o: eratosthenes.c bitset.h
	$(CC) $(CFLAGS) -c -o $@ eratosthenes.c

# # bitset.o
# $(BUILD_DIR)/bitset.o: bitset.c
# 	$(CC) $(CFLAGS) -c -o $@ $^
