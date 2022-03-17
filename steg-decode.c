// steg-decode.c
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Ondřej Zobal, FIT
// Přeloženo: gcc 11.2

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


void print_hidden_message(struct ppm *image, bitset_t primes, int start_index) {
    assert(start_index < (int) bitset_size(primes));

    char c = 0;
    int pos = 0;
    for (bitset_index_t i = start_index; i < bitset_size(primes); i++){
        if (!bitset_getbit(primes, i)) {
            c |= (image->data[i+1] & 1) << pos++;

            if (pos == CHAR_BIT) {
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

    print_hidden_message(image, bitset, START_PRIME-1);

    bitset_free(bitset);
    ppm_free(image);
}
