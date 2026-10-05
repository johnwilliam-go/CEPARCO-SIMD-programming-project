#include <stddef.h>

void cFunc(size_t size, double arr[]) {
	for (size_t i = 0; i < size; i++) {
		arr[i] += 4.25;
	}
}