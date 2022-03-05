#include "bitset.c"
#include <stdio.h>

void find_fun(bitset_t *bitset) {
    int count = 0;
    for (bitset_index_t i = 1; i < bitset_size(bitset)/64; i++) {
        if(bitset[i] != 0) {
            fprintf(stderr, "Index: %ld\tse vyskytuje: %ld\n", i, bitset[i]);
            count++;
        }
    }
    fprintf(stderr, "Celkem: %d změněných čísel. Velikost je %ld.\n\n", count, bitset_size(bitset)/8);
}

void find_not_fun(bitset_t *bitset) {
    int count = 0;
    for (bitset_index_t i = 1; i < 31; i++) {
        if(bitset[i] != ~(bitset_t)0) {
            fprintf(stderr, "Index: %ld\tse vyskytuje: %ld\n", i, bitset[i]);
            count++;
        }
    }
    fprintf(stderr, "Celkem: %d změněných čísel. Velikost je %ld.\n\n", count, bitset_size(bitset)/8);
}
int main() {
    // Stack
    bitset_t bitset[30];
    bitset[29] = 69;
    printf("test: %d", bitset_create(bitset, 30));

    find_fun(bitset);
    bitset_setbit(bitset, 78, 1);
    find_fun(bitset);
    bitset_setbit(bitset, 0, 1);
    bitset_setbit(bitset, 2, 1);
    bitset_setbit(bitset, 0, 0);
    bitset_setbit(bitset, 1, 0);
    bitset_setbit(bitset, 2, 0);
    find_fun(bitset);
    bitset_setbit(bitset, 30*64-1, 1);
    find_fun(bitset);

    printf("Zero byte value: %d\n", bitset_getbit(bitset, 0));
    printf("Zero byte value: %d\n", bitset_getbit(bitset, 30*64-1));

    for (bitset_index_t i = 1; i < bitset_size(bitset); i++) {
        if (!((int) bitset_setbit(bitset, i, 1))) {
            fprintf(stderr, "Error writing on index: %d!!!!!!!!!!!!!!!!\n", i);
        }
    }
    bitset_setbit(bitset, 30*64-1, 0);

    find_not_fun(bitset);
}
