#include "bitset.c"
#include <stdio.h>

void find_fun(bitset_t bitset) {
    int count = 0;
    for (bitset_index_t i = 1; i < bitset_size(bitset)/64; i++) {
        if(bitset[i] != 0) {
            fprintf(stderr, "Index: %ld\tse vyskytuje: %ld\n", i, bitset[i]);
            count++;
        }
    }
    fprintf(stderr, "Celkem: %d změněných čísel. Velikost je %ld.\n\n", count, bitset_size(bitset)/8);
}

void find_not_fun(bitset_t bitset) {
    int count = 0;
    for (bitset_index_t i = 1; i < 31; i++) {
        if(bitset[i] != ~(bitset_index_t)0) {
            fprintf(stderr, "Index: %ld\tse vyskytuje: %ld\n", i, bitset[i]);
            count++;
        }
    }
    fprintf(stderr, "Celkem: %d změněných čísel. Velikost je %ld.\n\n", count, bitset_size(bitset)/8);
}
int main() {
    #define s 100

    bitset_alloc(polo, s);

    printf("%d\n", bitset_getbit(polo, 6));
    bitset_setbit(polo, 41, 1);
    printf("%d\n", bitset_getbit(polo, 6));

}
