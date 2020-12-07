//Author: Sujoy Datta(NOV 30)
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>
int heathens=0, prudes=0;
char status[]="neutral";
sem_t heathenTurn, prudeTurn, heathenQueue, prudeQueue;
sem_t mutex;
void *heathen(void *params)
{
	int f= (int)params;
	sem_wait(&heathenTurn);
	sem_post(&heathenTurn);
	sem_wait(&mutex);
	printf("%d is ready to cross.\n",f);
	heathens++;

	if(strcmp(status,"neutral")==0)
	{
		strcpy(status,"heathens rule");
		sem_post(&mutex);
	}
	else if(strcmp(status,"prudes rule")==0)
	{
		if(heathens>prudes)
		{
			strcpy(status,"transition to heathens");
			sem_wait(&prudeTurn);
		}
		sem_post(&mutex);
		sem_wait(&heathenQueue);
	}
	else if(strcmp(status,"transition to heathens")==0)
	{
		sem_post(&mutex);
		sem_wait(&heathenQueue);
	}
	else
		sem_post(&mutex);

	printf("[+] %d is crossing the field.\n",f);
	sem_wait(&mutex);
	heathens--;

	if(heathens==0)
	{
		if(strcmp(status,"transition to prudes")==0)
			sem_post(&prudeTurn);
		if(prudes)
		{
			sem_post(&prudeQueue);
			strcpy(status,"prudes rule");
		}
		else
			strcpy(status,"neutral");
	}

	if(strcmp(status,"heathens rule")==0)
	{
		if(prudes>heathens)
		{
			strcpy(status,"transition to prudes");
			sem_wait(&heathenTurn);
		}
	}
	printf("Crossed.\n\n");
	sem_post(&mutex);
	pthread_exit(0);
}
int main(int argc, char const *argv[])
{
	sem_init(&mutex,0,1);
	sem_init(&heathenTurn,0,1);
	sem_init(&prudeTurn,0,1);
	sem_init(&heathenQueue,0,0);
	sem_init(&prudeQueue,0,0);
	pthread_t tid[20];
	for (int i = 0; i < 20; ++i)
	{
		pthread_create(&tid[i],NULL,heathen,(void *)(i+1));
	}
	for (int i = 0; i < 20; ++i)
	{
		pthread_join(tid[i],NULL);
	}
	return 0;
}