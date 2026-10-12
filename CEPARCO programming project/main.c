#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

#define x 1 << 20
#define y 1 << 26
#define z 1 << 28

#define INPUTS_SEED 2
#define DISPLAY_LIMIT 5

//extern void cFunc(size_t, double[]);
//extern void noSIMDFunc(size_t, double[]);
extern void xmmFunc(size_t, float[], float[], float[], float[]);
extern void cFunc(size_t, float[], float[], float[], float[]);
extern void ymmFunc(size_t, float[], float[], float[], float[]);
//extern void ymmFunc(size_t, double[]);


float* initializeZeroes(size_t size) {
	float* a = calloc(size, sizeof(float));
	for (size_t i = 0; i < size; i++) {
		a[i] = 0;
	}
	return a;
}

float* initializeB(size_t size) {
	float* b = calloc(size, sizeof(float));
	for (size_t i = 0; i < size; i++) {
		b[i] = tanhf(i); //arctanh
	}
	return b;
}

float* initializeC(size_t size) {
	float* c = calloc(size, sizeof(float));
	for (size_t i = 0; i < size; i++) {
		c[i] = atanf(i); //arctan
	}
	return c;
}

float* initializeD(size_t size) {
	float* d = calloc(size, sizeof(float));
	for (size_t i = 0; i < size; i++) {
		d[i] = acoshf(i + 1); //arccosh domain is [1,inf);
	}
	return d;
}

float** initializeArray(size_t size) {
	float** arr = calloc(4, sizeof(float*));

	arr[0] = initializeZeroes(size);
	arr[1] = initializeB(size);
	arr[2] = initializeC(size);
	arr[3] = initializeD(size);

	return arr;
}

double executeAndTimeFunc(void (*func)(size_t size, float arrA[], float arrB[], float arrC[], float arrD[]), size_t size, float arrA[], float arrB[], float arrC[], float arrD[]) {
	LARGE_INTEGER frequency, start, end;
	QueryPerformanceFrequency(&frequency);

	QueryPerformanceCounter(&start);
	func(size, arrA, arrB, arrC, arrD);
	QueryPerformanceCounter(&end);

	// Get runtime in seconds
	return (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
}

int testOutputs(size_t size, float cArr[], float asmArr[]) {
	int errors = 0;

	for (size_t i = 0; i < size; i++) {
		if (cArr[i] != asmArr[i]) {
			errors++;
		}
	}

	return errors;
}


int main() {
	float** cArray = initializeArray(x);
	float** xmmArray = initializeArray(x);
	float** ymmArray = initializeArray(x);

	double cRuntime = executeAndTimeFunc(cFunc, x, cArray[0], cArray[1], cArray[2], cArray[3]);
	double xmmRuntime = executeAndTimeFunc(xmmFunc, x, xmmArray[0], xmmArray[1], xmmArray[2], xmmArray[3]);
	double ymmRuntime = executeAndTimeFunc(ymmFunc, x, ymmArray[0], ymmArray[1], ymmArray[2], ymmArray[3]);
	xmmFunc(x, xmmArray[0], xmmArray[1], xmmArray[2], xmmArray[3]);
	ymmFunc(x, ymmArray[0], ymmArray[1], ymmArray[2], ymmArray[3]);
	cFunc(x, cArray[0], cArray[1], cArray[2], cArray[3]);

	printf("C vs Assembly vs XMM vs YMM Runtimes: %.2lfms vs %.2lfms vs %.2lfms\n", cRuntime, xmmRuntime, ymmRuntime);
	printf("No. of errors     (XMM): %d\n", testOutputs(x, cArray[0], xmmArray[0]));
	printf("No. of errors     (YMM): %d\n", testOutputs(x, cArray[0], ymmArray[0]));
	printf("+-------------+--------------------+-------------+-------------+\n");
	printf("| Optimized C | Assembly (No SIMD) |     XMM     |     YMM     |\n");
	printf("+-------------+--------------------+-------------+-------------+\n");
	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
		printf("| %11.2f |        placeholder | %11.2f | %11.2f |\n", cArray[0][i], xmmArray[0][i], ymmArray[0][i]);
	}
	printf("|         .   |                .   |         .   |         .   |\n");
	printf("|         .   |                .   |         .   |         .   |\n");
	printf("|         .   |                .   |         .   |         .   |\n");
	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
		printf("| %11.2f |        placeholder | %11.2f | %11.2f |\n", cArray[0][x -1 - i], xmmArray[0][x -1 - i], ymmArray[0][x -1 - i]);
	}
	printf("+-------------+--------------------+-------------+-------------+\n");

	free(cArray[0]);
	free(cArray[1]);
	free(cArray[2]);
	free(cArray[3]);
	free(xmmArray[0]);
	free(xmmArray[1]);
	free(xmmArray[2]);
	free(xmmArray[3]);
	free(ymmArray[0]);
	free(ymmArray[1]);
	free(ymmArray[2]);
	free(ymmArray[3]);
	return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <Windows.h>
//
// #define n 1 << 24 // 2^24
// #define INPUTS_SEED 2
// #define DISPLAY_LIMIT 5
//
// extern void cFunc(size_t, double[]);
// extern void noSIMDFunc(size_t, double[]);
// extern void xmmFunc(size_t, double[]);
// extern void ymmFunc(size_t, double[]);
//
// double* initializeDoubleArr(size_t size, int seed) {
// 	double* arr = calloc(size, sizeof(double));
//
// 	for (size_t i = 0; i < size; i++) {
// 		arr[i] = i * seed;
// 	}
//
// 	return arr;
// }
//
// double executeAndTimeFunc(void (*func)(size_t size, double arr[]), size_t size, double arr[]) {
// 	LARGE_INTEGER frequency, start, end;
// 	QueryPerformanceFrequency(&frequency);
//
// 	QueryPerformanceCounter(&start);
// 	func(size, arr);
// 	QueryPerformanceCounter(&end);
//
// 	// Get runtime in seconds
// 	return (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
// }
//
// int testOutputs(size_t size, double cArr[], double asmArr[]) {
// 	int errors = 0;
//
// 	for (size_t i = 0; i < size; i++) {
// 		if (cArr[i] != asmArr[i]) {
// 			errors++;
// 		}
// 	}
//
// 	return errors;
// }
//
//
// int main() {
// 	double* cArr = initializeDoubleArr(n, INPUTS_SEED);
// 	double* noSIMDArr = initializeDoubleArr(n, INPUTS_SEED);
// 	double* xmmArr = initializeDoubleArr(n, INPUTS_SEED);
// 	double* ymmArr = initializeDoubleArr(n, INPUTS_SEED);
//
// 	double cRuntime = executeAndTimeFunc(cFunc, n, cArr);
// 	double noSIMDRuntime = executeAndTimeFunc(noSIMDFunc, n, noSIMDArr);
// 	double xmmRuntime = executeAndTimeFunc(xmmFunc, n, xmmArr);
// 	double ymmRuntime = executeAndTimeFunc(ymmFunc, n, ymmArr);
//
// 	printf("C vs Assembly vs XMM vs YMM Runtimes: %.2lfms vs %.2lfms vs %.2lfms vs %.2lfms\n", cRuntime, noSIMDRuntime, xmmRuntime, ymmRuntime);
// 	printf("No. of errors (No SIMD): %d\n", testOutputs(n, cArr, noSIMDArr));
// 	printf("No. of errors     (XMM): %d\n", testOutputs(n, cArr, xmmArr));
// 	printf("No. of errors     (YMM): %d\n\n", testOutputs(n, cArr, ymmArr));
//
// 	printf("+-------------+--------------------+-------------+-------------+\n");
// 	printf("| Optimized C | Assembly (No SIMD) |     XMM     |     YMM     |\n");
// 	printf("+-------------+--------------------+-------------+-------------+\n");
//
// 	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
// 		printf("| %11.2lf |        %11.2lf | %11.2lf | %11.2lf |\n", cArr[i], noSIMDArr[i], xmmArr[i], ymmArr[i]);
// 	}
// 	printf("|         .   |                .   |         .   |         .   |\n");
// 	printf("|         .   |                .   |         .   |         .   |\n");
// 	printf("|         .   |                .   |         .   |         .   |\n");
// 	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
// 		printf("| %11.2lf |        %11.2lf | %11.2lf | %11.2lf |\n", cArr[n - i], noSIMDArr[n - i], xmmArr[n - i], ymmArr[n - i]);
// 	}
// 	printf("+-------------+--------------------+-------------+-------------+\n");
//
// 	free(cArr);
// 	free(noSIMDArr);
// 	free(xmmArr);
// 	free(ymmArr);
//
// 	return 0;
// }