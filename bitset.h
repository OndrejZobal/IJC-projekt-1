#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#define BITS_IN_BYTE 8
// Typ bitového pole (pro předávání parametru do funkce odkazem).
typedef unsigned long int *bitset_t;
// Typ indexu do bitového pole.
typedef unsigned long int bitset_index_t;

#ifndef USE_INLINE
#define bitset_size_to_bits(size) size / (sizeof(bitset_index_t) * BITS_IN_BYTE) + 1 + (size % (sizeof(bitset_index_t) * BITS_IN_BYTE) != 0)
#else
static inline bitset_index_t bitset_size_to_bits(size) {
    return size / (sizeof(bitset_index_t) * BITS_IN_BYTE) + 1 + (size % (sizeof(bitset_index_t) * BITS_IN_BYTE) != 0);
}
#endif

#define bitset_create(name, size) \
    static_assert(size > 0);                        \
    bitset_index_t name[bitset_size_to_bits(size)] = {size}



#define bitset_alloc(name, size)                                        \
    static_assert(size > 0);                                            \
    bitset_t name = calloc(bitset_size_to_bits(size), sizeof(bitset_index_t));                    \
    if (name == NULL) { \
        printf("cannot allocate memory!\n"); \
        exit(1); \
    } \
    name[0] = size

#ifndef USE_INLINE

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

// Inlines

/*
// FIXME
inline bitset_t bitset_alloc(bitset name, bitset_index_t size) {
    assert(size > 0);                                            
    name = calloc(bitset_size_to_bits(size), sizeof(bitset_index_t));                    
    if (name == NULL) { 
        printf("cannot allocate memory!\n"); \
        exit(1); 
    } 
    name[0] = size
    return name;
}
*/

#endif

static inline void bitset_free(bitset_t name) {
    free(name);
}

static inline bitset_index_t bitset_size(bitset_t array) { 
    return array[0];
}

static inline int bitset_setbit(bitset_t array, bitset_index_t index, int expression) {  
    // location is index + 1 long in bits (for storing the size).
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE; 

    // Location converted to index for long array.
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE); 
    
    // If location is larger or equal to the full size + size long.
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) { 
        return 0;
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
    return 1; 
}

static inline int bitset_getbit(bitset_t array, bitset_index_t index) {  
    // location is index + 1 long in bits (for storing the size).
    bitset_index_t location = index + sizeof(bitset_index_t) * BITS_IN_BYTE; 

    // Location converted to index for long array.
    int array_index = location / (sizeof(bitset_index_t) * BITS_IN_BYTE); 
    if (location >= bitset_size(array) + sizeof(bitset_index_t) * BITS_IN_BYTE) { 
        fprintf(stderr, "Borked!\n"); 
        exit(1);
    } 
    bitset_index_t value = array[array_index] >> (location % (sizeof(bitset_index_t) * BITS_IN_BYTE)); 
    return 1 & value; 
}

void bit_matrix(bitset_t numbers) {
    printf("\t\t1 2 3 4 5 6 7 8\n");
    printf("\t\t---------------\n");
    for(bitset_index_t i = 0; i < bitset_size(numbers)/8; i++) {
        printf("  %d - %d:\t", i*8, i*8+7);
        for(int j = 0; j < 8; j++) {
            printf("%d ", bitset_getbit(numbers, (i*8)+j));
        }
        printf("\n");
    }
    int reminder = bitset_size(numbers) % 8;
    printf("  %d - %d\t", bitset_size(numbers)-reminder, bitset_size(numbers)-1);
    for(int i = 0; i < reminder; i++) {
        printf("%d ", bitset_getbit(numbers, bitset_size(numbers)-reminder+i));
    }
    printf(reminder ? "END\n" : "\n");
}

