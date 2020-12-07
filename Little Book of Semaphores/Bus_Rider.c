//Author: Sujoy Datta(NOV 30)
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
sem_t bus,boarded,mutex;
int waiting=0;
void *bus_fun(void *params)
{
	int f=(int)params;
	sem_wait(&mutex);
	printf("Bus %d has arrived.\n",f);
	int n;
	if(waiting<50) n= waiting;
	else
		n=50;
	for (int i = 0; i < n; ++i)
	{
		sem_post(&bus);
		sem_wait(&boarded);
	}
	if((waiting-50)>0) waiting=waiting-50;
	else
		waiting=0;
	printf("Bus %d is departing.\n",f);
	sem_post(&mutex);
	pthread_exit(0);
}
void *rider(void *params)
{
	int f=(int)params;
	sem_wait(&mutex);
	waiting++;
	sem_post(&mutex);
	sem_wait(&bus);
	printf("[+]Passenger %d has boarded.\n",f);
	sem_post(&boarded);
	pthread_exit(0);

}
int main(int argc, char const *argv[])
{
	sem_init(&bus,0,0);
	sem_init(&boarded,0,0);
	sem_init(&mutex,0,1);
	pthread_t buss[20],riderr[20];
	for (int i = 0; i < 20; ++i)
	{
		pthread_create(&buss[i],NULL,bus_fun,(void *)(i+1));
		pthread_create(&riderr[i],NULL,rider,(void *)(i+1));
	}
	for (int i = 0; i < 20; ++i)
	{
		pthread_join(buss[i],NULL);
		pthread_join(riderr[i],NULL);
	}
	return 0;
}