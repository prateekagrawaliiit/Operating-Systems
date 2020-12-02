/*
* @Author: prateek
* @Date:   2020-11-30 16:26:15
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-30 16:39:46
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

int servings = 10;
sem_t mutex, empty_pot, full_pot;


void *cookerThread(void *param)
{
    while (1)
    {
        sem_wait(&empty_pot);
        printf("The cooker is filling the pot\n");
        sem_post(&full_pot);
    }

}
void *savageThread(void *param)
{
    while (1)
    {
        int f = (int *)param;

        sem_wait(&mutex);
        if (servings == 0)
        {
            sem_post(&empty_pot);
            sem_wait(&full_pot);
            servings = 10;
        }
        servings--;
        printf("Savage %d got a serving from the pot\n",f);
        sem_post(&mutex);

        printf("Savage %d is eating from the pot\n",f);
    }
}



int main(int argc, char* argv[])
{

    sem_init(&mutex, 0, 1);
    sem_init(&empty_pot, 0, 0);
    sem_init(&full_pot, 0, 0);

    pthread_t cookThread;
    pthread_create(&cookThread, NULL, cookerThread, NULL);


    pthread_t savages[5];
    for(int i=0;i<5;i++)
    {
        pthread_create(&savages[i],NULL,savageThread,(void *)(i+1));
    }
    for (int i = 0; i < 5; ++i)
    {
        pthread_join(savages[i],NULL);
    }
    return 0;

}
