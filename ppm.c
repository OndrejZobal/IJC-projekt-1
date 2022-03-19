// ppm.c
// Řešení IJC-DU1, příklad b), 20.3.2022
// Autor: Ondřej Zobal, FIT
// Přeloženo: gcc 11.2

#include "ppm.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <error.h>

#define MODUL_NAME "ppm"
#define PPM_MAGIC_NUMBER "P6"
#define COLOR_CHANNELS 3
#define SUPPORTED_MAX_VAL "255"
#define SIZE_INCREMENT 8

extern void warning_msg(const char *fmt, ...);
extern void error_exit(const char *fmt, ...);

#define WHITESPACE_SIZE 5
const char whitespace[WHITESPACE_SIZE] = {'\0', ' ', '\t', '\r', '\n'};

void add_char(char **string, int *size, int *length, const char c) {
    // Checking bounds (size-1 accounts for terminator)
    if(*length+1 >= *size-1) {
        *size += SIZE_INCREMENT;
        *string = realloc(*string, sizeof(string) * *size);
        if (*string == NULL) {
            // FIXME add actual error message
            error_exit("%s: Nelze alokovat paměť na hromadě!", MODUL_NAME);
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
        error_exit("%s Nelze alokovat paměť na hromadě!", MODUL_NAME);
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
    FILE *file = fopen(filename, "rb");

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
        warning_msg("%s: Soubor \"%s\" nelze otevřít!", MODUL_NAME, filename);
        goto error;
    }

    while ((c = getc(file)) != EOF) {
        switch (s) {
            // Checking if the file has a supported magic number.
            case MAGIC_NUMBER:
                // Condition for transitioning into another state.
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Exception for multiple white characters at the beggining of a line.
                    if(!string_length){
                        continue;
                    }
                    // Check if the filetype is supported
                    if(strcmp(PPM_MAGIC_NUMBER, string)) {
                        warning_msg("%s: Formát obrázku \"%s\" není podporován! Program podporuje formát PPM %s!", MODUL_NAME, filename, PPM_MAGIC_NUMBER);
                        goto error;
                    }
                    s = WIDTH;
                    reset_string(&string, &string_size, &string_length);
                    break;
                }
                add_char(&string, &string_size, &string_length, c);
                break;

            // Processing the width parameter of the image.
            case WIDTH:
                // Condition for transitioning into another state.
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Exception for multiple white characters at the beggining of a line.
                    if(!string_length){
                        continue;
                    }
                    // Check if the filetype is supported
                    char *ptr = NULL;
                    if(string_length < 1) {
                        warning_msg("%s: Obrázku \"%s\" má neplatnou šířku!", MODUL_NAME, filename);
                        goto error;
                    }
                    int width = strtol(string, &ptr, 10);
                    if(width == 0) {
                        warning_msg("%s: Obrázku \"%s\" má neplatnou šířku!", MODUL_NAME, filename);
                        goto error;
                    }

                    image->ysize = width;
                    s = HEIGHT;
                    reset_string(&string, &string_size, &string_length);
                    break;
                }

                add_char(&string, &string_size, &string_length, c);
                break;

            // Processing the height of the image.
            case HEIGHT:
                // Condition for transitioning into another state.
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Exception for multiple white characters at the beggining of a line.
                    if(!string_length){
                        continue;
                    }
                    // Check if the filetype is supported
                    char *ptr = NULL;
                    if(!string_length) {
                        warning_msg("%s: Obrázku \"%s\" má neplatnou výšku!", MODUL_NAME, filename);
                        goto error;
                    }
                    int height = strtol(string, &ptr, 10);
                    if(height == 0) {
                        warning_msg("%s: Obrázku \"%s\" má neplatnou výšku!", MODUL_NAME, filename);
                        goto error;
                    }

                    image->xsize = height;
                    s = MAX_VALUE;
                    reset_string(&string, &string_size, &string_length);
                    break;
                }

                add_char(&string, &string_size, &string_length, c);
                break;

            // Processing the max color value. Usually 255.
            case MAX_VALUE:
                // Condition for transitioning into another state.
                if(isCharInArray(c, whitespace, WHITESPACE_SIZE)) {
                    // Exception for multiple white characters at the beggining of a line.
                    if(!string_length){
                        continue;
                    }

                    if(strcmp(string, SUPPORTED_MAX_VAL)) {
                        warning_msg("%s: Program nezná barevné kódování obrázku \"%s\" (%s), podporované (%s)!", MODUL_NAME, filename, string, SUPPORTED_MAX_VAL);
                        goto error;
                    }

                    s = DATA;
                    free(string);
                    string = NULL;
                    string_size = image->xsize * image->ysize * COLOR_CHANNELS;
                    string_length = 0;

                    image = realloc(image, sizeof(image) + string_size);
                    if (!image) {
                        error_exit("%s: Nelze alokovat paměť na hromadě!", MODUL_NAME);
                        goto error;
                    }

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
                    warning_msg("%s: Obrázek \"%s\" má příliš mnoho obrazových dat! (odčekáváno pouze %dB)", MODUL_NAME, filename, string_size);
                    goto error;
                }

                image->data[string_length++] = c;
                break;
        }
    }

    if (string_length != string_size) {
        // FIXME use error
        warning_msg("%s: Obrázek \"%s\" postrádá data! (%dB z odčekáváných %dB)", MODUL_NAME, filename, string_length, string_size);
        goto error;
    }

    fclose(file);
    return image;

error:
    if(string != NULL) free(string);
    if(image != NULL) free(image);
    if(file != NULL) fclose(file);
    return NULL;
}

void ppm_free(struct ppm *p) {
    free(p);
}
