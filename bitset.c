#include "bitset.h"
#include <stdlib.h>

#define bitset_create(array, size) ({ \
    for(bitset_index_t i = 1; i < size; i++) { \
        array[i] = 0;                          \
    }                                          \
    array[0] = size;                           \
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

#define bitset_size(array) ( array[0] )

#define bitset_setbit(array, index, expression) ({  \
    int status = 1; \
    index++; \
    if (index >= bitset_size(array)) { \
        status = 0; \
    } \
    else {                                      \
        array[index] = expression;\
    } \
    /* TODO is this needed?? */ \
    index--; \
    status; \
})

#define error_exit(message) ({ \
    fprintf(stderr, message); \
    exit(1); \
})

#define bitset_getbit(array, index) ({  \
    index++; \
    if (index >= bitset_size(array)) { \
        error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, (unsigned long)mez); \
    } \
    array[index]; \
})
