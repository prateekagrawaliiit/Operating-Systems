//Author: Sujoy Datta(NOV 30)
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
sem_t multiplex;
pthread_mutex_t mutex;
void *runner(void *params)
{
	int f=(int)params;
	//setting value of semaphore to 3
	sem_post(&multiplex);
	sem_post(&multiplex);
	sem_post(&multiplex);
	pthread_mutex_lock(&mutex);
	printf("Adult %d is taking care of children.\n",f);
		sem_wait(&multiplex);
		printf("Child 1\n");
		sem_wait(&multiplex);
		printf("Child 2\n");
		sem_wait(&multiplex);
		printf("Child 3\n");
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

int main(int argc, char const *argv[])
{
	sem_init(&multiplex,0,1);
	pthread_mutex_init(&mutex, NULL);
	pthread_t tid[20];
	for (int i = 0; i < 20; ++i)
	{
		pthread_create(&tid[i],NULL,runner,(void *)(i+1));
	}
	for (int i = 0; i < 20; ++i)
	{
		pthread_join(tid[i],NULL);
	}
	return 0;
}