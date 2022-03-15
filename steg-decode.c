#include "ppm.h"
#include "bitset.h"
#include "eratosthenes.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define START_PRIME 29
#define COLOR_CHANNELS 3
#define SECRET_MESSAGE_BUFFER_SIZE 300
#define MODULE_NAME "steg-decode"

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

void find_hidden_message(struct ppm *image, bitset_t primes, int start_index) {
    assert(start_index < (int) bitset_size(primes));

    char c = 0;
    int pos = 0;
    for (bitset_index_t i = start_index; i < bitset_size(primes); i++){
        if (!bitset_getbit(primes, i)) {
            c |= (image->data[i+1] & 1) << pos++;

            if (pos == BITS_IN_BYTE) {
                if(c == '\0') {
                    printf("\n");
                    return;
                }

                printf("%c", c);
                pos = 0;
                c = 0;
            }
        }
    }

    printf("\n");
}

int main(int argc, char **argv){
    if (argc != 2) {
        fprintf(stderr, "Špatné argumenty");
        exit(1);
    }
    struct ppm *image = ppm_read(*(argv+1));

    if (image == NULL) {
        error_exit("%s: Obrázek nelze načíst!", MODULE_NAME);
    }

    bitset_alloc(bitset, image->xsize * image->ysize * COLOR_CHANNELS+1);

    Eratosthenes(bitset);

    find_hidden_message(image, bitset, START_PRIME-1);

}
