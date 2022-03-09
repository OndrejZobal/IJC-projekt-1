#include "bitset.h"
#include <stdio.h>

void find_1(bitset_t pole) {
    for(bitset_index_t i = 0; i < bitset_size(pole); i++) {
        int bit = bitset_getbit(pole, i);
        if (bit) {
            printf("Index: %ld obsahuje jednicku!\n", i);
        }
    }
}

void find_sus(bitset_t pole) {
    for(bitset_index_t i = 0; i < bitset_size(pole); i++) {
        printf("Index: %ld\n", i);
        bitset_setbit(pole, i, 1);
        for(bitset_index_t j = 0; j < bitset_size(pole); j++) {
            if (j != i && bitset_getbit(pole, j)) {
                printf("%ld, %ld, %d\n", i, j, bitset_getbit(pole, j));
            }
        }
        bitset_setbit(pole, i, 0);
    }
}

int main() {
    #define s 100
    bitset_alloc(pole, s);
    // for(bitset_index_t i = 0; i < bitset_size(pole); i++) {
    //     bitset_setbit(pole, i, 1);
    //     bit_matrix(pole);
    //     // bitset_setbit(pole, i, 1);
    // }
    bit_matrix(pole);
    printf("bude nula: %d\n", bitset_getbit(pole, 55));
    bitset_setbit(pole, 31, 1);
    printf("bude jedncka ale nemel by: %d\n", bitset_getbit(pole, 55));
    bit_matrix(pole);
    bitset_setbit(pole, 55, 0);
    printf("bude nula: %d\n", bitset_getbit(pole, 31));
    bit_matrix(pole);
    bitset_setbit(pole, 32, 1);
    bitset_setbit(pole, 33, 1);
    bit_matrix(pole);

    bitset_free(pole);
}
