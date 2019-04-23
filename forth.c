#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define THREADS 3
#define N 1E9

double start=0;
double finish=1;
double step;

typedef struct {double A,B;} pth;//replaced `int by `double

double atandiff(double x) {
	return 1.0 / (1 + x*x);
}

void* thread_func(void* arg) {
	pth *init = (pth*) arg;

	double *S = (double*)malloc(sizeof(double));

	for (double x = init->A; x < init->B; x = x + step) {
		*S = *S +0.5 * step * (atandiff(x) + atandiff(x+step));
	}
	printf("Sum is: %.10f\n",*S);
	return S;
}

int main() {
	step = (finish - start) / N;
	double sum = 0;
	pth args[THREADS];
	pthread_t th[THREADS];
	void *thread_return[THREADS];

	for (int i = 0; i < THREADS; i++) {
		args[i].A = start + (finish - start) / THREADS * i;
		args[i].B = start + (finish - start) / THREADS * (i + 1);
		pthread_create(&th[i], NULL, thread_func, (void*) &args[i]);
	}

	for(int i = 0; i < THREADS; i++) {
		pthread_join(th[i], (&thread_return[i]));
	}

	for (int i = 0; i < THREADS; i++) {
		sum += *(double *) thread_return[i];
	}

	printf("SUM=%16.15f\n", sum);
	printf("Precision=%16.15f\n", 4 * sum - M_PI);

	return 0;
}
