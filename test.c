#include "bitset.c"
#include <stdio.h>

int main() {
    // Stack
    bitset_t bitset[30];
    bitset[29] = 69;
    printf("test: %d", bitset_create(bitset, 30));

    printf("Delka bitsetu %ld\n", bitset_size(bitset));

}
