#include <pthread.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define SQUARES_NUM 100
#define THREADS 2

int squares_count = 0;
int squares[SQUARES_NUM];
pthread_mutex_t lock;


void* worker(void* arg) {
	int thread = (long) arg;
	int number;
	int root;
	while(1) {
		number = rand();
		root = sqrt(number);
		if (root == ceil(root)) {
			pthread_mutex_lock(&lock);
			printf("Thread number %d asquired a lock to write number %d, that root is %d to %d cell\n",
				thread, number, root, squares_count);
			if (squares_count == SQUARES_NUM) {
				pthread_mutex_unlock(&lock);
				break;
			} else {
				squares[squares_count] = number;
				squares_count++;
			}
			pthread_mutex_unlock(&lock);
		}
	}
}


int main() {
	pthread_t th[THREADS];
	pthread_mutex_init(&lock, NULL);
	for (int i = 0; i < THREADS; i++) {
		pthread_create(&th[i], NULL, worker, (void*) (long) i);
	}
	for (int i = 0; i < THREADS; i++) {
		pthread_join(th[i], NULL);
	}
	return 0;
}

