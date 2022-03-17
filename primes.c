// primes.c
// Řešení IJC-DU1, příklad a), 20.3.2022
// Autor: Ondřej Zobal, FIT
// Přeloženo: gcc 11.2

#include <stdio.h>
#include <time.h>
#include "eratosthenes.h"

#define LIMIT 300000000
#define PRINT_LIMIT 10

void print_last_primes(bitset_t pole, int print_limit) {
    bitset_index_t last_numbers[print_limit];
    // Printing last primes.
    bitset_index_t j = 0;
    for (bitset_index_t i = bitset_size(pole)-1; i > 0 && j < PRINT_LIMIT; i--) {
        if (!bitset_getbit(pole, i)) {
            last_numbers[j++] = i+1;
        }
    }

    for (int i = print_limit-1; i >= 0; i--) {
        printf("%ld\n", last_numbers[i]);
    }
}

int main() {
    clock_t start = clock();
    bitset_alloc(numbers, LIMIT);

    Eratosthenes(numbers);

    print_last_primes(numbers, PRINT_LIMIT);

    bitset_free(numbers);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}
