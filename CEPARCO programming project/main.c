#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

#define n_20 (1 << 20)
#define n_26 (1 << 26)
#define n_28 (1 << 28)

#define INPUTS_SEED 2
#define DISPLAY_LIMIT 5

double time_elapsed_cfunc[30];
double time_elapsed_noSimd[30];
double time_elapsed_xmm[30];
double time_elapsed_ymm[30];
//extern void cFunc(size_t, double[]);
//extern void noSIMDFunc(size_t, double[]);
extern void xmmFunc(size_t, float[], float[], float[], float[]);
extern void cFunc(size_t, float[], float[], float[], float[]);
extern void ymmFunc(size_t, float[], float[], float[], float[]);
//extern void ymmFunc(size_t, double[]);

double averageTime(const double t[]) {
	double sum = 0;
	for (int i = 0; i < 30; i++) {
		sum += t[i];
	}
	return sum / 30;
}

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
		b[i] = sinf(i+0.001f);
	}
	return b;
}

float* initializeC(size_t size) {
	float* c = calloc(size, sizeof(float));
	for (size_t i = 0; i < size; i++) {
		c[i] = cosf(i + 0.002f);
	}
	return c;
}

float* initializeD(size_t size) {
	float* d = calloc(size, sizeof(float));
	for (size_t i = 0; i < size; i++) {
		d[i] = sinf(i + 0.003f); 
	}
	return d;
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

void runPerformanceTests(size_t samples) {

	printf("\nInitializing arrays with %zu elements... Please wait...\n", samples);
	float* B = initializeB(samples);
	float* C = initializeC(samples);
	float* D = initializeD(samples);
	float* A_c = calloc(samples, sizeof(float));
	// float* A_asm = calloc(samples, sizeof(float)); not yet implemented
	float* A_xmm = calloc(samples, sizeof(float));
	float* A_ymm = calloc(samples, sizeof(float));

	printf("Arrays initialized... Running Performance Tests...\n");

	for (int i = 0; i < 30; i++) {
		time_elapsed_cfunc[i] = executeAndTimeFunc(cFunc, samples, A_c, B, C, D);
		time_elapsed_xmm[i] = executeAndTimeFunc(xmmFunc, samples, A_xmm, B, C, D);
		time_elapsed_ymm[i] = executeAndTimeFunc(ymmFunc, samples, A_ymm, B, C, D);
	}

	if (samples == n_20){
		printf("Performance tests for vector sizes of 2^20 elements\n");
	}
	else if (samples == n_26){
		printf("Performance tests for vector sizes of  2^26 elements\n");
	}
	else if (samples == n_28){
		printf("Performance tests for vector sizes of 2^28 elements\n");
	}

	printf("C vs Assembly vs XMM vs YMM average runtimes (n=30): %.2lfms vs %.2lfms vs %.2lfms\n", averageTime(time_elapsed_cfunc), averageTime(time_elapsed_xmm), averageTime(time_elapsed_ymm));
	// add nyo nalang for no simd
	// printf("(no SIMD) Correct values: %d  | Incorrect Values: %d\n", samples - testOutputs(samples, A_c, A_noSIMD), testOutputs(samples, A_c, A_noSIMD));
	printf("(XMM) Correct values: %d  | Incorrect Values: %d\n", samples-testOutputs(samples, A_c, A_xmm), testOutputs(samples, A_c, A_xmm));
	printf("(YMM) Correct values: %d  | Incorrect Values: %d\n", samples-testOutputs(samples, A_c, A_ymm), testOutputs(samples, A_c, A_ymm));
	
	
	printf("+-------------+--------------------+-------------+-------------+\n");
	printf("| Optimized C | Assembly (No SIMD) |     XMM     |     YMM     |\n");
	printf("+-------------+--------------------+-------------+-------------+\n");
	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
		printf("| %11.2f |        placeholder | %11.2f | %11.2f |\n", A_c[i], A_xmm[i], A_ymm[i]);
	}
	printf("|         .   |                .   |         .   |         .   |\n");
	printf("|         .   |                .   |         .   |         .   |\n");
	printf("|         .   |                .   |         .   |         .   |\n");
	for (size_t i = 0; i < DISPLAY_LIMIT; i++) {
		printf("| %11.2f |        placeholder | %11.2f | %11.2f |\n", A_c[samples - 1 - i], A_xmm[samples - 1 - i], A_ymm[samples - 1 - i]);
	}
	printf("+-------------+--------------------+-------------+-------------+\n");

	free(A_c);
	free(A_xmm);
	free(A_ymm);
	free(B);
	free(C);
	free(D);
}

int main() {
	runPerformanceTests(n_20);
	runPerformanceTests(n_26);
	runPerformanceTests(n_28);
	return 0;
}