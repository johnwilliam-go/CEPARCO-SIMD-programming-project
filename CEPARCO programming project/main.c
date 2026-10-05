#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define n 1 << 24 // 2^24
#define INPUTS_SEED 2
#define DISPLAY_LIMIT 5

extern void cFunc(size_t, double[]);
extern void noSIMDFunc(size_t, double[]);
extern void xmmFunc(size_t, double[]);
extern void ymmFunc(size_t, double[]);

double* initializeDoubleArr(size_t size, int seed) {
	double* arr = calloc(size, sizeof(double));

	for (size_t i = 0; i < size; i++) {
		arr[i] = i * seed;
	}

	return arr;
}

double executeAndTimeFunc(void (*func)(size_t size, double arr[]), size_t size, double arr[]) {
	LARGE_INTEGER frequency, start, end;
	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&start);
	func(size, arr);
	QueryPerformanceCounter(&end);

	// Get runtime in seconds
	return (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
}

int testOutputs(size_t size, double cArr[], double asmArr[]) {
	int errors = 0;

	for (size_t i = 0; i < size; i++) {
		if (cArr[i] != asmArr[i]) {
			errors++;
		}
	}

	return errors;
}


int main() {
	double* cArr = initializeDoubleArr(n, INPUTS_SEED);
	double* noSIMDArr = initializeDoubleArr(n, INPUTS_SEED);
	double* xmmArr = initializeDoubleArr(n, INPUTS_SEED);
	double* ymmArr = initializeDoubleArr(n, INPUTS_SEED);

	double cRuntime = executeAndTimeFunc(cFunc, n, cArr);
	double noSIMDRuntime = executeAndTimeFunc(noSIMDFunc, n, noSIMDArr);
	double xmmRuntime = executeAndTimeFunc(xmmFunc, n, xmmArr);
	double ymmRuntime = executeAndTimeFunc(ymmFunc, n, ymmArr);

	printf("C vs Assembly vs XMM vs YMM Runtimes: %.2lfms vs %.2lfms vs %.2lfms vs %.2lfms\n", cRuntime, noSIMDRuntime, xmmRuntime, ymmRuntime);
	printf("No. of errors (No SIMD): %d\n", testOutputs(n, cArr, noSIMDArr));
	printf("No. of errors     (XMM): %d\n", testOutputs(n, cArr, xmmArr));
	printf("No. of errors     (YMM): %d\n\n", testOutputs(n, cArr, ymmArr));

	printf("+-------------+--------------------+-------------+-------------+\n");
	printf("| Optimized C | Assembly (No SIMD) |     XMM     |     YMM     |\n");
	printf("+-------------+--------------------+-------------+-------------+\n");

	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
		printf("| %11.2lf |        %11.2lf | %11.2lf | %11.2lf |\n", cArr[i], noSIMDArr[i], xmmArr[i], ymmArr[i]);
	}
	printf("|         .   |                .   |         .   |         .   |\n");
	printf("|         .   |                .   |         .   |         .   |\n");
	printf("|         .   |                .   |         .   |         .   |\n");
	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
		printf("| %11.2lf |        %11.2lf | %11.2lf | %11.2lf |\n", cArr[n - i], noSIMDArr[n - i], xmmArr[n - i], ymmArr[n - i]);
	}
	printf("+-------------+--------------------+-------------+-------------+\n");

	free(cArr);
	free(noSIMDArr);
	free(xmmArr);
	free(ymmArr);

	return 0;
}