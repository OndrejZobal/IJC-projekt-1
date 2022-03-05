#include "bitset.h"
#include <stdlib.h>

#define bitset_create(array, size) ({ \
    for(bitset_index_t i = 1; i < size; i++) { \
        array[i] = 0;                          \
    }                                          \
    array[0] = size-1;                         \
})

#define bitset_new_create(name, size) ({ \
    bitset_t name[size]; \
    \
    for(bitset_index_t i = 1; i < size; i++) { \
        name[i] = 0;                          \
    }                                          \
    name[0] = size-1;                         \
    name; \
})

// FIXME
#define bitset_alloc(array, size) ({  \
    bitset_t *array = malloc(size * sizeof(bitset_t)); \
    if(array == NULL) { \
        fprintf(stderr, "bitset_alloc: Chyba alokace paměti"); \
        exit(1); \
    } \
    \
    bitset_create(array, size); \
})

#define bitset_size(array) ( array[0] * sizeof(bitset_t) * BITS_IN_BYTE )

#define bitset_setbit(array, index, expression) ({  \
    int status = 1; \
    /* Calculationg the index in the long int array. */ \
    bitset_index_t location = index + sizeof(bitset_t) * BITS_IN_BYTE; \
    int array_index = location / (sizeof(bitset_t) * BITS_IN_BYTE); \
    \
    if (location >= bitset_size(array) + sizeof(bitset_t) * BITS_IN_BYTE) { \
        status = 0; \
    } \
    else {                                      \
        int val; \
        val = 1 << (location % (sizeof(bitset_t) * BITS_IN_BYTE)); \
        if (expression != 0) { \
            array[array_index] = array[array_index] | val; \
        } \
        else { \
            val = ~val; \
            array[array_index] = array[array_index] & val; \
        } \
    } \
    /* TODO is this needed?? */ \
    status; \
})

#define error_exit(message) ({ \
    fprintf(stderr, message); \
    exit(1); \
})

#define bitset_getbit(array, index) ({  \
    bitset_index_t location = index + sizeof(bitset_t) * BITS_IN_BYTE; \
    int array_index = location / (sizeof(bitset_t) * BITS_IN_BYTE); \
    int value; \
    if (location >= bitset_size(array) * sizeof(bitset_t)) { \
        fprintf(stderr, "Borked!"); \
        exit(1);\
    } \
    else { \
        value = array[array_index] >> (location % (sizeof(bitset_t) * BITS_IN_BYTE)); \
        value = ((bitset_t) 1) & value; \
    } \
    value; \
})
