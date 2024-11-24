#include "bit_array.h"
#include "assert.h"
#include "string.h"

/* Create a new bit array with a given size */
BitArray *BitArray_new(size_t size)
{
    /* Allocate memory for the struct */
    BitArray *bit_array = malloc(sizeof(BitArray));
    assert(bit_array != NULL);

    /* Allocate memory for the bit array */ 
    /* Note: (size + 7) / 8 == ceil(size / 8) */
    bit_array->words = calloc((size + 7) / 8, sizeof(unsigned char));
    assert(bit_array->words != NULL);

    bit_array->size = size;

    return bit_array;
}

/* Expand bit array if it is full */
static void resize(BitArray *bit_array)
{
    /* Expand array */
    size_t new_size = bit_array->size * 2 + 1; /* Double array size */
    size_t old_size = bit_array->size;

    /* Reallocate memory */
    unsigned char *new_arr = realloc(bit_array->words, (new_size + 7) / 8);
    assert(new_arr != NULL);

    /* Initialize new bits to 0 */
    memset(new_arr + (old_size + 7) / 8, 0, (new_size - old_size + 7) / 8);

    bit_array->words = new_arr;
    bit_array->size = new_size;
}

/* Set the bit at index i to 1 */
void BitArray_set(BitArray *bit_array, size_t i)
{
    /* Expand bit array i is out of range (should rarely happen) */
    if (i == bit_array->size) {
        resize(bit_array);
    }
    assert(i < bit_array->size);

    /*  */
    bit_array->words[i / 8] |= (1 << (i % 8));
}

/* Check if the bit at index i is set to 1 */
int BitArray_get(BitArray *bit_array, size_t i)
{
    assert(i < bit_array->size);
    /* Checks if (i % 8)th bit in (i/8)th word is 1 */
    return (bit_array->words[i / 8] >> (i % 8)) & 1;
}

/* Free the memory allocated for bit array */
void BitArray_free(BitArray *bit_array)
{
    free(bit_array->words);
    free(bit_array);
}