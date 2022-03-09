#include "bitset.h"
#include <math.h>
#include <stdio.h>

#define LIMIT 3000000

// FIXME čtyřicet dvojka nějakým způsobem odjevábá sedimčku.
// TODO předělej to na inliny a vydebuguj to reee

void Eratosthenes(bitset_t pole) {
    #define PRINT_LIMIT 10
    bitset_index_t last_primes[PRINT_LIMIT] = {0};
    for (bitset_index_t i = 1; i < bitset_size(pole); i++) {
        unsigned int bit = bitset_getbit(pole, i);
        bitset_setbit(pole, 0, 1);
        //printf("index %ld, bit of num seven: %d\n", i, bitset_getbit(pole, 6));
        if(!bit) {
            for (int j = 1; j < PRINT_LIMIT; j++) {
                last_primes[j-1] = last_primes[j];
            }

            last_primes[PRINT_LIMIT-1] = i+1;
            for (bitset_index_t j = 1; j < bitset_size(pole); j++) {
                bitset_setbit(pole, j*(i+1)-1, 1);
            }
        }
    }

    for(int i = 0; i < PRINT_LIMIT; i++) {
        printf("%ld\n", last_primes[i]);
    }
}

int main() {
    bitset_alloc(numbers, LIMIT);

    Eratosthenes(numbers);

    bitset_free(numbers);
}
