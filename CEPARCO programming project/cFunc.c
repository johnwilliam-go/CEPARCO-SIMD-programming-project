#include <stddef.h>

void cFunc(size_t size, float arrA[], float arrB[], float arrC[], float arrD[]) {
    for (size_t i = 0; i < size; i++) {
        arrA[i] = arrB[i] + arrC[i] * arrD[i];
    }
}