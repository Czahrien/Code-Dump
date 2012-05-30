#include "mergesort.h"

int int_cmp(const void* i1, const void* i2) {
    return *((int*)i1) <= *((int*)i2);
}

int float_cmp(const void* f1, const void* f2) {
    return *((double*)f1) <= *((double*)f2);
}

int double_cmp(const void* d1, const void* d2) {
    return *((double*)d1) <= *((double*)d2);
}

void merge( void* a1, size_t n1, void* a2, size_t n2, size_t size_elements, int (*cmp_func)(const void*, const void*)) {

    uint8_t* new_array = (uint8_t*)malloc((n1+n2)*size_elements);
    int i1 = 0;
    int i2 = 0;
    int si = 0;
    while( i1 < n1 && i2 < n2 ) {
        if( cmp_func((uint8_t*)a1+i1*size_elements,(uint8_t*)a2+i2*size_elements)) {
            memcpy(new_array+si*size_elements,(uint8_t*)a1+i1*size_elements, size_elements);
            i1++;
        } else {
            memcpy(new_array+si*size_elements,(uint8_t*)a2+i2*size_elements, size_elements);
            i2++;
        }
        si++;
    }
    while( i1 < n1 ) {
            memcpy(new_array+si*size_elements,(uint8_t*)a1+i1*size_elements, size_elements);
            si++; i1++;
    }
    while( i2 < n2 ) {
            memcpy(new_array+si*size_elements,(uint8_t*)a2+i2*size_elements, size_elements);
            si++; i2++;
    }
    memcpy(a1, new_array,(n1+n2)*size_elements);
    free(new_array);
}

void mrgsrt( void* elements, size_t num_elements, size_t size_elements, int (*cmp_func)(const void*, const void*) ) {
    if( num_elements > 1 ) {
        mrgsrt(elements, num_elements/2, size_elements, cmp_func);     
        mrgsrt((uint8_t*)elements+size_elements*(num_elements/2), num_elements - num_elements/2, size_elements, cmp_func);     
        merge(elements, num_elements/2, (uint8_t*)elements + size_elements*(num_elements/2), num_elements - num_elements/2, size_elements, cmp_func);
    }
}
