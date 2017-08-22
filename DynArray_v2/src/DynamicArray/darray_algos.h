#ifndef DARRAY_ALGOS_H_
#define DARRAY_ALGOS_H_

#include <DynamicArray/darray.h>
#include <stdlib.h>
typedef int (*DArray_compare)(const Item* a, const Item* b);

int DArray_qsort(DArray* array, DArray_compare cmp);

// int DArray_heapsort(DArray* array, DArray_compare cmp);

// int DArray_mergesort(DArray* array, DArray_compare cmp);

#endif
