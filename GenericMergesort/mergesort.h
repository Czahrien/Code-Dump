// Author: Benjamin Mayes
// Description: Contains a declaration of the mergesort routine and some support routines.
#ifndef MERGESORT_H_
#define MERGESORT_H_
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
int int_cmp(const void* i1, const void* i2);
int float_cmp(const void* f1, const void* f2);
int double_cmp(const void* d1, const void* d2);
void merge( void* array_1, size_t a1_size, void* array_2, size_t a2_size, size_t element_size );
void mrgsrt( void* elements, size_t num_elements, size_t size_elements, int (*cmp_func)(const void*, const void*));
#endif
