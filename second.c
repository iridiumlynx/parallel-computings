#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define COLS 4
#define ROWS 3
#define THREADS 10

long FIRST_MATRIX[ROWS][COLS],SECOND_MATRIX[ROWS][COLS],RESULT_MATRIX[ROWS][COLS];

void printMatrix(long* matrix, int rows, int cols) {
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			printf("matrix[%d][%d]=%ld ", i, j, *((matrix + i*rows) + j));
		}
		printf("\n");
	}
}

void* thread_function(void* arg){
	int thread_number = (long)arg;
	if (thread_number < ROWS){
		for (int i=0; i<COLS; i++){
			RESULT_MATRIX[thread_number][i] = FIRST_MATRIX[thread_number][i] + SECOND_MATRIX[thread_number][i];
		}
	}
	else {
		printf("I am useless with number %d\n", thread_number);
	}
	return NULL;
}

int main() {

	int i, j, state;

	for (i=0; i<ROWS; i++){
		for (j=0; j<COLS; j++){
			FIRST_MATRIX[i][j] = j + i * COLS;
		}
	}

	printMatrix((long*)FIRST_MATRIX, ROWS, COLS);

	for (i=0; i<ROWS; i++){
		for (j=0; j<COLS; j++){
			SECOND_MATRIX[i][j]=(3-j)+(2-i)*COLS;
		}
	}

	printMatrix((long*)SECOND_MATRIX, ROWS, COLS);

	pthread_t th[THREADS];
	pthread_attr_t attr[THREADS];

	for (i=0; i<THREADS; i++){
		pthread_attr_init(&attr[i]);
		pthread_attr_setstacksize(&attr[i], 2000000000);
		// pthread_attr_setdetachstate(&attr[i], PTHREAD_CREATE_DETACHED);
		pthread_attr_getdetachstate(&attr[i], &state);
		printf("thread with number %d will be created as %s \n", i, state == PTHREAD_CREATE_DETACHED ? "detached" : "joinable");
		pthread_create(&th[i], &attr[i], thread_function, (void *) (long)i);
	}
	sleep(20);
	for (i=0; i<THREADS; i++) pthread_join(th[i], NULL);
	printMatrix((long*)RESULT_MATRIX, ROWS, COLS);
}
