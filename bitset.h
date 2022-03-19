// bitset.h
// Řešení IJC-DU1, příklad a), 20.3.2022
// Autor: Ondřej Zobal, FIT
// Přeloženo: gcc 11.2

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include "error.h"

#define MODUL_NAME "bitset"

extern void error_exit(const char *fmt, ...);
extern void warning_msg(const char *fmt, ...);

// Typ bitového pole (pro předávání parametru do funkce odkazem).
typedef unsigned long int *bitset_t;
// Typ indexu do bitového pole.
typedef unsigned long int bitset_index_t;

// Converts an amount of bitset_t array members into bits.
#define bitset_size_to_bits(size) size / (sizeof(unsigned long) * CHAR_BIT) + 1 + (size % (sizeof(unsigned long) * CHAR_BIT) != 0)

#define bitset_create(name, size) \
    static_assert(size > 0);                        \
    bitset_index_t name[bitset_size_to_bits(size)] = {size}

#define bitset_alloc(name, size)                                        \
    assert(size > 0);                                            \
    bitset_t name = calloc(bitset_size_to_bits(size), sizeof(unsigned long));                    \
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
    bitset_index_t location = index + sizeof(unsigned long) * CHAR_BIT;\
    \
    int array_index = location / (sizeof(unsigned long) * CHAR_BIT);\
    \
    if (location >= bitset_size(array) + sizeof(unsigned long) * CHAR_BIT) {\
        /* FIXME use error.h */ \
        error_exit("%s: Byly překročeny hranice pole!", MODUL_NAME); \
    }\
    else {\
        bitset_index_t val = 1L << (location % (sizeof(unsigned long) * CHAR_BIT));\
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
    (index + sizeof(unsigned long) * CHAR_BIT >= bitset_size(array) + sizeof(unsigned long) * CHAR_BIT) ? \
    error_exit("%s: Byly překročeny hranice pole!", MODUL_NAME),1 :\
    /* lisp moment */ \
    (int) (1 & (array[/*array_index */ ((/*location: */ index + sizeof(unsigned long) * CHAR_BIT) / (sizeof(unsigned long) * CHAR_BIT))] >> ((/* location: */ index + sizeof(unsigned long) * CHAR_BIT) % ( /* size in bits */ sizeof(unsigned long) * CHAR_BIT)))) \
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
    bitset_index_t location = index + sizeof(unsigned long) * CHAR_BIT;

    // Location converted to index for long array.
    int array_index = location / (sizeof(unsigned long) * CHAR_BIT);
    
    // If location is larger or equal to the full size + size long.
    if (location >= bitset_size(array) + sizeof(unsigned long) * CHAR_BIT) {
        // FIXME use error.h
        error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array));
    }
    else {                                      
        bitset_index_t val = 1L << (location % (sizeof(unsigned long) * CHAR_BIT));
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
    bitset_index_t location = index + sizeof(unsigned long) * CHAR_BIT;

    // Location converted to index for long array.
    int array_index = location / (sizeof(unsigned long) * CHAR_BIT);
    if (location >= bitset_size(array) + sizeof(unsigned long) * CHAR_BIT) {
        //fprintf(stderr, "Borked!\n");
        error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(array));
    }
    bitset_index_t value = array[array_index] >> (location % (sizeof(unsigned long) * CHAR_BIT));
    return 1 & value;
}

#endif
#endif
