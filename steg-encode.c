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
#define SIZE_INCREMENT 8
#define MODULE_NAME "steg-encode"


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
}

int hide_message(struct ppm *image, const char* message, bitset_t hiding_spots, int start_index) {
    assert(start_index < (int) bitset_size(hiding_spots));

    int message_counter = 1;
    int pos = 0;
    char c = message[0];
    for(bitset_index_t i = start_index-1; i > sizeof(hiding_spots); i++) {
        // If this index insn't a hiding spot, continue.
        if(bitset_getbit(hiding_spots, i)) {
            continue;
        }

        int bit = (c >> pos) & 1;

        if(bit) {
            image->data[i+1] |= bit;
        } else {
            image->data[i+1] &= (c >> pos) & (~((char)1));
        }
        pos++;

        if(pos == CHAR_BIT) {
            // If last char was 0, the message is fully encoded.
            printf("char: %c\n", c);
            if(c == '\0') {
                return 1;
            }
            c = message[message_counter++];
            pos = 0;
        }
    }

    return 0;
}

char *read_message(char *filename) {
    int string_size = 8;
    int string_length = 0;
    char *string = malloc(sizeof(char) * string_size);
    string[0] = '\0';

    FILE *file = fopen(filename, "r");
    if(!file) {
        error_exit("Soubor \"%s\" se nepodařilo najít", filename);
    }

    int c = 0;
    while((c = getc(file)) != EOF) {
        add_char(&string, &string_size, &string_length, c);
    }

    return string;
}

int main(int argc, char **argv){
    if(argc != 4) {
        error_exit("Šbatné argumenty! Zadejte: <zdrojový obrázek> <tajná zpráva> <zakódovaný obrázek>");
    }

    struct ppm *image = ppm_read(argv[1]);
    bitset_alloc(pole, image->xsize * image->ysize * 3);
    Eratosthenes(pole);
    char *message = read_message(argv[2]);
    hide_message(image, message, pole, 29);
    ppm_write(image, argv[3]);
    bitset_free(pole);
    ppm_free(image);
}
