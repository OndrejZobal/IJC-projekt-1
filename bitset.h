#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <error.h>

extern void warning_msg(const char *fmt, ...);

#define BITS_IN_BYTE 8
#define MODUL_NAME "bitset"
// Typ bitového pole (pro předávání parametru do funkce odkazem).
typedef unsigned long int *bitset_t;
// Typ indexu do bitového pole.
typedef unsigned long int bitset_index_t;

extern void error_exit(const char *fmt, ...);
extern void warning_msg(const char *fmt, ...);

#define bitset_size_to_bits(size) size / (sizeof(bitset_index_t) * BITS_IN_BYTE) + 1 + (size % (sizeof(bitset_index_t) * BITS_IN_BYTE) != 0)

#define bitset_create(name, size) \
    static_assert(size > 0);                        \
    bitset_index_t name[bitset_size_to_bits(size)] = {size}

#define bitset_alloc(name, size)                                        \
    assert(size > 0);                                            \
    bitset_t name = calloc(bitset_size_to_bits(size), sizeof(bitset_index_t));                    \
    if (name == NULL) { \
        error_exit("%s: Nelze alokovat paměť na hromadě!", MODUL_NAME); \
    } \
    name[0] = size

/*
 * Makros
 */

#ifndef USE_INLINE

#define bitset_free(name) free(name)

#define bitset_size(array) array[0]

#define bitset_setbit(array, index, expression) do {  \
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE;\
    \
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE);\
    \
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) {\
        /* FIXME use error.h */ \
        error_exit("%s: Byly překročeny hranice pole!", MODUL_NAME); \
    }\
    else {\
        bitset_index_t val = 1L << (location % (sizeof(bitset_index_t) * BITS_IN_BYTE));\
        if (expression) {\
            array[array_index] |= val;\
        }\
        else {\
            val = ~val;\
            array[array_index] &= val;\
        }\
    } \
} while(0);

#define bitset_getbit(array, index) ( \
    (index + sizeof(bitset_index_t) * BITS_IN_BYTE >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) ? \
    error_exit("%s: Byly překročeny hranice pole!", MODUL_NAME),1 :\
    /* lisp moment */ \
    (int) (1 & (array[/*array_index */ ((/*location: */ index + sizeof(bitset_index_t) * BITS_IN_BYTE) / (sizeof(bitset_index_t) * BITS_IN_BYTE))] >> ((/* location: */ index + sizeof(bitset_index_t) * BITS_IN_BYTE) % ( /* size in bits */ sizeof(bitset_index_t) * BITS_IN_BYTE)))) \
    )

/*
 * Inlines
*/

#else

#ifndef BITSET_INCLUDE_ONCE
#define BITSET_INCLUDE_ONCE 1

inline void bitset_free(bitset_t name) {
    free(name);
}

inline bitset_index_t bitset_size(bitset_t array) {
    return array[0];
}

inline void bitset_setbit(bitset_t array, bitset_index_t index, int expression) {
    // location is index + 1 long in bits (for storing the size).
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE; 

    // Location converted to index for long array.
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE); 
    
    // If location is larger or equal to the full size + size long.
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) { 
        // FIXME use error.h
        error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array));
    }
    else {                                      
        bitset_index_t val = 1L << (location % (sizeof(bitset_index_t) * BITS_IN_BYTE)); 
        if (expression) { 
            array[array_index] |= val; 
        } 
        else { 
            val = ~val; 
            array[array_index] &= val; 
        } 
    } 
    /* TODO is this needed?? */ 
}


inline int bitset_getbit(bitset_t array, bitset_index_t index) {
    // location is index + 1 long in bits (for storing the size).
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE; 

    // Location converted to index for long array.
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE); 
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) { 
        //fprintf(stderr, "Borked!\n");
        error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array));
    }
    bitset_index_t value = array[array_index] >> (location % (sizeof(bitset_index_t) * BITS_IN_BYTE));
    return 1 & value;
}

#endif
#endif
