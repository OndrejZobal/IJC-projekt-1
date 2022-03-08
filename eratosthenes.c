#include "bitset.c"
#include <math.h>
#include <stdio.h>

#define LIMIT 100

void Eratosthenes(bitset_t pole) {
    unsigned int limit = (unsigned int)sqrt(bitset_size(pole))+1;

    for (bitset_index_t i = 1; i < bitset_size(pole); i++) {
        unsigned int bit = bitset_getbit(pole, i);
        bitset_setbit(pole, 0, 1);
        if(!bit) {
            printf("%ld\n", i+1);
            for (bitset_index_t j = i; j < bitset_size(pole); j+=i+1) {
                int reality_check = bitset_getbit(pole, j);
                bitset_setbit(pole, j, 1);
                if (bitset_getbit(pole, j) != 1) {
                    printf("THIS IS BAD!!! %ld\n", j);
                }
            }
        }
    }
}

int main() {
    bitset_alloc(numbers, LIMIT);

    Eratosthenes(numbers);

    bitset_free(numbers);
}
