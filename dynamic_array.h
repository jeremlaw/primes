#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

typedef struct Array *Array;

extern Array Prime_Array();
extern void Array_push(Array array, int elem);
extern int Array_at(Array array, int index);
extern int Array_size(Array array);
extern void Array_free(Array array);
extern int *getArr(Array array);

#endif