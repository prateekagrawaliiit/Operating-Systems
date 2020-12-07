/*
* @Author: prateek
* @Date:   2020-11-30 15:16:20
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-30 15:47:10
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

int eating = 0, ready_to_leave = 0;
sem_t mutex, ok_to_leave;

void *dine(void *params)
{
    int f = (int *)params;
    printf("Diner %d got the food.\n", f);
    sem_wait(&mutex);
    eating++;
    if (eating == 2 && ready_to_leave == 1)
    {
        sem_post(&ok_to_leave);
        ready_to_leave--;
    }
    sem_post(&mutex);
    printf("Diner %d is dining.\n", f);
    eating--;
    ready_to_leave++;
    if(eating==1 && ready_to_leave==1)
    {
        sem_post(&mutex);
        sem_wait(&ok_to_leave);
    }
    else if(eating==0 && ready_to_leave==2)
    {
        sem_post(&ok_to_leave);
        ready_to_leave-=2;
        sem_post(&mutex);
    }
    else
    {
        ready_to_leave--;
        sem_post(&mutex);
    }

    pthread_exit(0);
}

int main()
{
    sem_init(&mutex,0,1);
    sem_init(&ok_to_leave,0,0);
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