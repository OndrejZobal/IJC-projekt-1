#include "ppm.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PPM_MAGIC_NUMBER "P6"
#define COLOR_CHANNELS 3

#define SIZE_INCREMENT 8
#define WHITESPACE_SIZE 5

const char whitespace[WHITESPACE_SIZE] = {'\0', ' ', '\t', '\r', '\n'};

void add_char(char **string, int *size, int *length, const char c) {
    // Checking bounds (size-1 accounts for terminator)
    if(*length+1 >= *size-1) {
        *size += SIZE_INCREMENT;
        *string = realloc(*string, sizeof(string) * *size);
        if (*string == NULL) {
            // FIXME add actual error message
            fprintf(stderr, "ppm: Nelze alokovat paměť na hromadě!\n");
            error(1);
        }
    }
    (*string)[(*length)++] = c;
    (*string)[(*length)] = '\0';
}

void reset_string(char **string, int *size, int *length) {
    if (*string != NULL) {
        free(*string);
    }
    *size = SIZE_INCREMENT;
    *length = 0;

    *string = malloc(sizeof(char) * *size);
    if (*string == NULL) {
        printf("big bad");
        exit(1);
    }
    *string[0] = '\0';
}

bool isCharInArray(const char c, const char whitespace[], int size) {
    for (int i = 0; i < size; i++) {
        if (c == whitespace[i]) {
            return 1;
        }
    }
    return 0;
}

struct ppm *ppm_read(const char *filename) {
    FILE *file = fopen(filename, "r");

    enum stage {MAGIC_NUMBER, WIDTH, HEIGHT, MAX_VALUE, DATA};

    struct ppm *image = malloc(sizeof(image));

    // Creating a string for capturing the data.
    int string_size;
    int string_length;
    char *string = NULL;
    reset_string(&string, &string_size, &string_length);

    int c;
    enum stage s = MAGIC_NUMBER;

    if (file == NULL) {
        goto error;
    }

    while ((c = getc(file)) != EOF) {
        switch (s) {
            case MAGIC_NUMBER:
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Check if the filetype is supported
                    if(strcmp(PPM_MAGIC_NUMBER, string)) {
                        goto error;
                    }
                    s = WIDTH;
                    reset_string(&string, &string_size, &string_length);
                    break;
                }
                add_char(&string, &string_size, &string_length, c);
                break;

            case WIDTH:
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Check if the filetype is supported
                    char *ptr = NULL;
                    int width = strtol(string, &ptr, 10);
                    if(width == 0) {
                        fprintf(stderr, "Nulova sirka\n");
                        goto error;
                    }

                    image->ysize = width;
                    s = HEIGHT;
                    reset_string(&string, &string_size, &string_length);
                    break;
                }

                add_char(&string, &string_size, &string_length, c);
                break;

            case HEIGHT:
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Check if the filetype is supported
                    char *ptr = NULL;
                    int height = strtol(string, &ptr, 10);
                    if(height == 0) {
                        goto error;
                    }

                    image->xsize = height;
                    s = MAX_VALUE;
                    reset_string(&string, &string_size, &string_length);
                    break;
                }

                add_char(&string, &string_size, &string_length, c);
                break;

            // I do not need to capcure this value, so we are just skipping it.
            case MAX_VALUE:
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    s = DATA;
                    free(string);
                    string_size = image->xsize * image->ysize * COLOR_CHANNELS;
                    string_length = 0;
                    string = malloc(sizeof(char) * string_size);
                    break;
                }

                add_char(&string, &string_size, &string_length, c);
                break;

            case DATA:
                // string_size now stores the correct size of the data portion.
                // string_length stores the length of data that is currently stored in string.
                // We won't use add_char anymore, since the data in string is not a valid c string
                // and we also do not want to resize it.

                if (string_length+1 > string_size) {
                    // FIXME use error
                    fprintf(stderr, "ppm: Příliš mnoho obrazových dat!\n");
                    goto error;
                }

                string[string_length++] = c;
                break;
        }
    }

    if (string_length != string_size) {
        // FIXME use error
        fprintf(stderr, "ppm: Chybí obrazová data!\n");
        goto error;
    }

    #ifndef DEBUG
    fprintf(stderr, "Načten obrázek height: %d, width: %d, celkem %dB\n", image->xsize, image->ysize, string_length);
    #endif

    image->data = string;
    fclose(file);
    return image;

error:
    if(string != NULL) free(string);
    if(image != NULL) free(image);
    if(file != NULL) fclose(file);
    return NULL;
}
