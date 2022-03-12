#include "bitset.h"

void bit_matrix(bitset_t numbers) {
    printf("\t\t1 2 3 4 5 6 7 8\n");
    printf("\t\t---------------\n");
    for(bitset_index_t i = 0; i < bitset_size(numbers)/8; i++) {
        printf("  %ld - %ld:\t", i*8, i*8+7);
        for(int j = 0; j < 8; j++) {
            printf("%d ", bitset_getbit(numbers, (i*8)+j));
        }
        printf("\n");
    }
    int reminder = bitset_size(numbers) % 8;
    printf("  %ld - %ld\t", bitset_size(numbers)-reminder, bitset_size(numbers)-1);
    for(int i = 0; i < reminder; i++) {
        printf("%d ", bitset_getbit(numbers, bitset_size(numbers)-reminder+i));
    }
    printf(reminder ? "END\n" : "\n");
}
