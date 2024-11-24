#include "dynamic_array.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

struct Array {
    size_t size;
    size_t capacity;
    int *arr;
};

int *getArr(Array array)
{
    return array->arr;
}

static void error(bool condition, char *message)
{
    if (condition) {
        fprintf(stderr, "%s\n", message);
        assert(!condition);
    }
}

static void *allocate(size_t size)
{
    void *p = malloc(size);
    error(p == NULL, "Memory allocation failed");
    return p;
}

static Array Array_new(size_t capacity)
{
    Array array = allocate(sizeof(struct Array));
    array->arr = allocate(capacity * sizeof(int));
    array->size = 0;
    array->capacity = capacity;
    return array;
}

Array Prime_Array()
{
    Array array = Array_new(2);
    Array_push(array, 2);
    Array_push(array, 3);
    return array;
}

void Array_free(Array array)
{
    free(array->arr);
    array->arr = NULL;
    array->size = 0;
    array->capacity = 0;
    free(array);
}

int Array_at(Array array, int index)
{
    error(index >= array->size || index < 0, "Index out of range");
    return (array->arr[index]);
}

int Array_size(Array array)
{
    return array->size;
}

static void resize(Array array)
{
    size_t new_capacity = array->capacity * 2 + 1;
    int *new_arr = realloc(array->arr, new_capacity * sizeof(int));
    error(new_arr == NULL, "Memory reallocation failed");
    array->arr = new_arr;
    array->capacity = new_capacity;
}

void Array_push(Array array, int elem)
{
    if (array->size == array->capacity) {
        resize(array);
    }
    array->arr[array->size] = elem;
    array->size++;
}