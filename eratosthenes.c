#include "bitset.h"
#include <math.h>
#include <stdio.h>

#define LIMIT 300000000
#define PRINT_LIMIT 10

// FIXME čtyřicet dvojka nějakým způsobem odjevábá sedimčku.
// TODO předělej to na inliny a vydebuguj to reee

void Eratosthenes(bitset_t pole) {
    // Calculating primes.
    for (bitset_index_t i = 1; i < bitset_size(pole); i++) {
        unsigned int bit = bitset_getbit(pole, i);
        bitset_setbit(pole, 0, 1);
        if(!bit) {
            for (bitset_index_t j = i+i+2; j < bitset_size(pole)+1; j+=i+1) {
                bitset_setbit(pole, j-1, 1);
            }
        }
    }

    // Printing last primse.
    // TODO print is in reverse order!!!!
    bitset_index_t j = 0;
    for (bitset_index_t i = bitset_size(pole)-1; i > 0 && j < PRINT_LIMIT; i--) {
        if (!bitset_getbit(pole, i)) {
            printf("%ld\n", i+1);
            j++;
        }
    }
}

int main() {
    bitset_alloc(numbers, LIMIT);

    Eratosthenes(numbers);

    bitset_free(numbers);
}
