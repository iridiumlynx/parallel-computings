#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PRODUCERS 2
#define CONSUMERS 1
#define DELAY 5
#define LENGTH 10

int rear = 0, front = 0;
int buf[LENGTH];
sem_t empty;
sem_t full;
pthread_mutex_t reading;
pthread_mutex_t writing;

void* producer_work(void *arg) {
	long number = (long) arg;
	printf("Producer with number %ld started\n", number);
	int data;
	while(1) {
		data = rand() % 11 - 5;
		sem_wait(&empty);
		pthread_mutex_lock(&writing);
		buf[rear] = data;
		rear = (rear + 1) % LENGTH;
		printf("Producer with number %ld send data: %d\n", number, data);
		pthread_mutex_unlock(&writing);
		sem_post(&full);
		sleep(DELAY);
	}
	return NULL;
}

void* consumer_work(void *arg) {
	long number = (long) arg;
	printf("Consumer with number %ld started\n", number);
	int data;
	while(1) {
		sem_wait(&full);
		pthread_mutex_lock(&reading);
		data = buf[front];
		front = (front + 1) % LENGTH;
		pthread_mutex_unlock(&reading);
		printf("Consumer with number %ld get data: %d\n", number, data);
		sem_post(&empty);
		sleep(DELAY);
	}
	return NULL;
}



int main() {
	pthread_t PR[PRODUCERS];
	pthread_t CS[CONSUMERS];

	sem_init(&empty, 0, LENGTH);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&reading, NULL);
	pthread_mutex_init(&writing, NULL);

	for (int i = 0; i < PRODUCERS; i++) {
		pthread_create(&PR[i], NULL, producer_work, (void*) (long)i);
	}

	for (int i = 0; i < CONSUMERS; i++) {
		pthread_create(&CS[i], NULL, consumer_work, (void*) (long)i);
	}

	consumer_work((void*)99);

	return 0;
}
