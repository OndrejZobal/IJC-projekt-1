CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra
BUILD_DIR = .
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


.PHONY: build run clean primes-i-full

# Builds both variants of the program
build: primes primes-i $(BUILD_DIR)/steg-decode

# Builds and runs both variants of the program
run: build
	$(BUILD_DIR)/primes
	$(BUILD_DIR)/primes-i
	$(BUILD_DIR)/steg-decode $(STEGFILE)

# Removes objects and executable files from BUILD_DIR.
clean:
	find $(BUILD_DIR) -maxdepth 1 -type f -executable -exec rm {} +
	rm $(BUILD_DIR)/*.o

# Calls another instance of make and builds primes with inline functions
primes-i: bitset.h
	make debug=$(debug) USE_INLINE=1 primes-i-full

# primes (exec), calls primes.o, eratosthenes.o and bitset.o
primes-i-full: $(BUILD_DIR)/primes$(SUFFIX).o $(BUILD_DIR)/eratosthenes$(SUFFIX).o
	$(CC) $(CFLAGS) -o primes-i $^ $(LIBS)

primes: $(BUILD_DIR)/primes$(SUFFIX).o $(BUILD_DIR)/eratosthenes$(SUFFIX).o
	$(CC) $(CFLAGS) -o primes $^ $(LIBS)

$(BUILD_DIR)/error.o: error.c error.h
	$(CC) $(CFLAGS) -c -o $@ error.c

# primes.o
$(BUILD_DIR)/primes$(SUFFIX).o: primes.c error.o eratosthenes.h $(BUILD_DIR)/error.o
	$(CC) $(CFLAGS) -c -o $@ primes.c

# eratosthenes.o
$(BUILD_DIR)/eratosthenes$(SUFFIX).o: eratosthenes.c bitset.h $(BUILD_DIR)/error.o
	$(CC) $(CFLAGS) -c -o $@ eratosthenes.c

# B
$(BUILD_DIR)/ppm.o: ppm.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(BUILD_DIR)/steg-decode: $(BUILD_DIR)/ppm.o steg-decode.c $(BUILD_DIR)/eratosthenes$(SUFFIX).o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# # bitset.o
# $(BUILD_DIR)/bitset.o: bitset.c
# 	$(CC) $(CFLAGS) -c -o $@ $^

