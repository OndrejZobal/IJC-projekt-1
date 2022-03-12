#include <math.h>
#include "eratosthenes.h"

// FIXME čtyřicet dvojka nějakým způsobem odjevábá sedimčku.
// TODO předělej to na inliny a vydebuguj to reee

void Eratosthenes(bitset_t pole) {
    // Calculating primes.
    for (bitset_index_t i = 1; i < sqrt(bitset_size(pole)); i++) {
        unsigned int bit = bitset_getbit(pole, i);
        bitset_setbit(pole, 0, 1);
        if(!bit) {
            for (bitset_index_t j = i+i+2; j < bitset_size(pole)+1; j+=i+1) {
                bitset_setbit(pole, j-1, 1);
            }
        }
    }
}
