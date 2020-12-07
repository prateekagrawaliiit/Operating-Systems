//Author: Sujoy Datta(NOV 30)
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
sem_t mutex, okToLeave;
int eating=0, readyToLeave=0;
void *dine(void *params)
{
	int f=(int *)params;
	printf("Diner %d got food.\n",f);
	sem_wait(&mutex);
	eating++;
	if (eating==2 && readyToLeave==1)
	{
		sem_post(&okToLeave);
		readyToLeave--;
	}
	sem_post(&mutex);
	printf("[+]Diner %d is dining.\n",f);
	sem_wait(&mutex);
	eating--;
	readyToLeave++;

	if (eating==1 && readyToLeave==1)
	{
		sem_post(&mutex);
		sem_wait(&okToLeave);
	}
	else if(eating==0 && readyToLeave==2)
	{
		sem_post(&okToLeave);
		readyToLeave-=2;
		sem_post(&mutex);
	}
	else
	{
		readyToLeave--;
		sem_post(&mutex);
	}
	pthread_exit(0);
}
int main(int argc, char const *argv[])
{
	sem_init(&mutex,0,1);
	sem_init(&okToLeave,0,0);
	pthread_t diners[20];
	for (int i = 0; i < 20; ++i)
	{
		pthread_create(&diners[i],NULL,dine,(void *)(i+1));
	}
	for (int i = 0; i < 20; ++i)
	{
		pthread_join(diners[i],NULL);
	}
	return 0;
}