#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define THREADS 3
#define N 1E8

double at=0;
double bt=1;
double h;

struct pth {int A,B;};

double f(double x) {
	return 4.0 / (1 + x*x);
}

void* thread_func(void* arg) {
	struct pth *init = (struct pth *)arg;

	double *S = (double*)malloc(sizeof(double));

	for (int i = init->A; i < init->B; i++) {
		*S = *S +0.5 * h * (f(i*h) + f((i+1)*h));
	}
	printf("Sum is: %.10f\n",*S);
	return S;
}

int main() {
	h = (bt - at) / N;
	double sum = 0;
	struct pth args[THREADS];
	pthread_t th[THREADS];
	void *thread_return[THREADS];

	for(int i = 0; i < THREADS; i++) {
		pthread_join(th[i], (&thread_return[i]));
	}

	for (int i = 0; i < THREADS; i++) {
		sum += *(double *) thread_return[i];
	}

	printf("SUM=%16.15f\n", sum);
	printf("Precision=%16.15f\n", sum - M_PI);

	return 0;
}

