#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H

#include <stdlib.h>

typedef struct {
    unsigned char *words; /* Pointer to array of 1 byte words */
    size_t size;         /* Number of bits */
} BitArray;

/* Constructor/Destructor */
extern BitArray *BitArray_new(size_t size);
extern void BitArray_free(BitArray *bit_array);

/* Functions to set/get bits */
extern void BitArray_set(BitArray *bit_array, size_t i);
extern void BitArray_clear(BitArray *bit_array, size_t i);
extern int BitArray_get(BitArray *bit_array, size_t i);
extern void BitArray_toggle(BitArray *bit_array, size_t index);

#endif