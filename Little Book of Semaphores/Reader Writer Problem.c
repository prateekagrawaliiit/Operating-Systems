/*
* @Author: prateek
* @Date:   2020-11-29 19:52:05
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-29 20:10:19
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

sem_t wrt;
pthread_mutex_t mutex;
int count = 1;
int numreader = 0;

void *runner_wrtr(void *param)
{
	sem_wait(&wrt);
	count = count * 2;
	printf("Writer %d modified the value of count to %d \n", (*(int *)param), count);
	sem_post(&wrt);
}

void *runner_reader(void *param)
{
	pthread_mutex_lock(&mutex);
	numreader++;
	if (numreader == 1)
	{
		sem_wait(&wrt);
	}
	pthread_mutex_unlock(&mutex);

	printf("Reader %d read the value of count as : %d\n", (*(int *)param), count);

	pthread_mutex_lock(&mutex);

	numreader--;
	if (numreader == 0)
		sem_post(&wrt);

	pthread_mutex_unlock(&mutex);

}

int main()
{
	pthread_t rid[10], wid[10];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&wrt, 0, 1);

	int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

	for (int i = 0; i < 10; i++)
	{
		pthread_create(&rid[i], NULL, (void *)runner_reader, (void *)&a[i]);
		pthread_create(&wid[i], NULL, (void *)runner_wrtr, (void *)&a[i]);

	}

	for (int i = 0; i < 10; i++)
	{
		pthread_join(rid[i], NULL);
		pthread_join(wid[i], NULL);

	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&wrt);



}