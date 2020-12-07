//Author: Sujoy Datta(NOV 30)
#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
sem_t agentSem, tobacco, paper, match;
void *agentA(void *params)
{
	sem_wait(&agentSem);
	printf("Agent A : I am giving away my matches.\n");
	sem_post(&tobacco);
	sem_post(&paper);
}
void *agentB(void *params)
{
	sem_wait(&agentSem);
	printf("Agent B : I am giving away my tobacco.\n");
	sem_post(&paper);
	sem_post(&match);
}
void *agentC(void *params)
{
	sem_wait(&agentSem);
	printf("Agent C : I am giving away my rolling paper.\n");
	sem_post(&tobacco);
	sem_post(&match);
}
void *matches(void *params)
{
	sem_wait(&tobacco);
	sem_wait(&paper);
	printf("Person A: I got matches :)\n");
	printf("--------\n");
	sem_post(&agentSem);
}
void *tobaccos(void *params)
{
	sem_wait(&paper);
	sem_wait(&match);
	printf("Person B: I got tobacco to smoke :)\n");
	printf("-----\n");
	sem_post(&agentSem);

}
void *papers(void *params)
{
	sem_wait(&tobacco);
	sem_wait(&match);
	printf("Person C: I got rolling paper :)\n");
	printf("-------\n");
	sem_post(&agentSem);
}
int main(int argc, char const *argv[])
{
	sem_init(&agentSem,0,1);
	sem_init(&tobacco,0,0);
	sem_init(&paper,0,0);
	sem_init(&match,0,0);
	pthread_t people[3],agents[3];
	pthread_create(&agents[0],NULL,agentA,(void *)NULL);
	pthread_create(&agents[1],NULL,agentB,(void *)NULL);
	pthread_create(&agents[2],NULL,agentC,(void *)NULL);
	pthread_create(&people[0],NULL,matches,(void *)NULL);
	pthread_create(&people[1],NULL,tobaccos,(void *)NULL);
	pthread_create(&people[2],NULL,papers,(void *)NULL);
	for (int i = 0; i < 3; ++i)
	{
		pthread_join(agents[i],NULL);
		pthread_join(people[i],NULL);
	}
	return 0;
}