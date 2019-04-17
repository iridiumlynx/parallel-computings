#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DIMENSION 4

double first_vector[DIMENSION];
double second_vector[DIMENSION];
int middle = DIMENSION / 2;

void* thread_func(void* arg)
{
	int local_counter, slice_start, slice_end;

	double * local_result = (double*) malloc(sizeof(double));

	*local_result = 0;

	slice_start = (long)arg == 1 ? 0 : middle;
	slice_end = (long)arg == 1 ? middle : DIMENSION;

	for (local_counter = slice_start; local_counter < slice_end; local_counter++)
	{
		*local_result = *local_result + first_vector[local_counter] * second_vector[local_counter];
	}

	printf("local_result=%f \n", *local_result);

	return local_result;
}

int main()
{
	for (int counter = 0; counter < DIMENSION; counter++)
	{
		first_vector[counter] = rand() % 10;
		second_vector[counter] = rand() % 10;
		printf("first_vector[%d]=%f \n", counter, first_vector[counter]);
		printf("second_vector[%d]=%f \n", counter, second_vector[counter]);
	}
	pthread_t thread1, thread2;

	void *result1, *result2;

	if (
		pthread_create(&thread1, NULL, thread_func, (void *)0)
		|| pthread_create(&thread2, NULL, thread_func, (void *)1)
	) {
		return EXIT_FAILURE;
	}

	pthread_join(thread1, &result1);
	pthread_join(thread2, &result2);

	double result = *(double *)result1 + *(double *)result2;
	free(result1);
	free(result2);

	printf("global_result=%f\n", result);

	return EXIT_SUCCESS;
}
