
#include <stdio.h>
#include "eratosthenes.h"

#define LIMIT 300000000
#define PRINT_LIMIT 10

void print_last_primes(bitset_t pole, int print_limit) {
    bitset_index_t last_numbers[print_limit];
    // Printing last primse.
    // TODO print is in reverse order!!!!
    bitset_index_t j = 0;
    for (bitset_index_t i = bitset_size(pole)-1; i > 0 && j < PRINT_LIMIT; i--) {
        if (!bitset_getbit(pole, i)) {
            //printf("%ld\n", i+1);
            last_numbers[j++] = i+1;
        }
    }

    for (int i = print_limit-1; i >= 0; i--) {
        printf("%ld\n", last_numbers[i]);
    }
}

int main() {
    bitset_alloc(numbers, LIMIT);

    Eratosthenes(numbers);

    print_last_primes(numbers, 10);

    bitset_free(numbers);
}