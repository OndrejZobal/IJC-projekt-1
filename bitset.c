#include "bitset.h"
#include <stdlib.h>
#include <assert.h>

#define bitset_size_to_bits(size) size / (sizeof(bitset_index_t) * BITS_IN_BYTE) + 1 + (size % (sizeof(bitset_index_t) * BITS_IN_BYTE) != 0)

#define bitset_create(name, size) \
    static_assert(size > 0);                        \
    bitset_index_t name[bitset_size_to_bits(size)] = {size}

// FIXME
#define bitset_alloc(name, size)                                        \
    static_assert(size > 0);                                            \
    bitset_t name = calloc(bitset_size_to_bits(size), sizeof(bitset_index_t));                    \
    if (name == NULL) { \
        printf("cannot allocate memory!\n"); \
        exit(1); \
    } \
    name[0] = size

#define bitset_free(name) free(name)

#define bitset_size(array) array[0]

#define bitset_setbit(array, index, expression) ({  \
    int status = 1; \
    /* Calculationg the index in the long int array. */ \
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE; \
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE); \
    \
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) { \
        status = 0; \
    } \
    else {                                      \
        int val; \
        val = 1 << (location % (sizeof(bitset_index_t) * BITS_IN_BYTE)); \
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
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE; \
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE); \
    int value; \
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) { \
        fprintf(stderr, "Borked!\n"); \
        exit(1);\
    } \
    else { \
        value = array[array_index] >> (location % (sizeof(bitset_index_t) * BITS_IN_BYTE)); \
        value = ((bitset_index_t) 1) & value; \
    } \
    value; \
})
