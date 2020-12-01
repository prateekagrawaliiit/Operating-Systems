/*
* @Author: prateek
* @Date:   2020-12-01 16:16:17
* @Last Modified by:   prateek
* @Last Modified time: 2020-12-01 16:16:19
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

int heathens_counter = 0; //number of heathens
int prude_counter = 0;    //number of prudes

//init mutex
pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t heathens_turn;
sem_t prudes_turn;

void *prudes()
{

    //Arrival of the queue, that is, the threads that were waiting to enter the critical zone, are arriving
    pthread_mutex_lock(&_mutex);
    sleep(1);

    prude_counter++;
    printf("\nA Prude has arrived\nPrudes: %d | Heathens: %d\n", prude_counter, heathens_counter);

    pthread_mutex_unlock(&_mutex);

    sleep(1);

    //Control time between prudes and heathens. Prevents an individual from requesting the mutex without the faction shift
    sem_wait(&prudes_turn);

    pthread_mutex_lock(&_mutex);
    prude_counter--; //crossing

    if (prude_counter)
    { //If there are no more prudes, prudes = 0
        if (heathens_counter)
        {                             //but there are heathens
            sem_post(&heathens_turn); //Release the heathens execution, it's heathens turn, green flag heathens to occupy the critical zone
        }
    }

    if (heathens_counter > prude_counter)
    {                             //If there are more heathens than prudes
        sem_post(&heathens_turn); //let one heathen cross
    }
    else
    { //if there are more prudes let the one prude cross
        sem_post(&prudes_turn);
    }

    printf("\nPRUDE leaves the critical zone \nno of remaining Prudes %d\nno of remaining Heathens %d\n", prude_counter, heathens_counter);

    pthread_mutex_unlock(&_mutex);
    pthread_exit(0);
}

void *heathens()
{

    //Llegada en fila de heathens
    pthread_mutex_lock(&_mutex);
    sleep(1);

    heathens_counter++; //llegada de un heathens a la fila
    printf("\nA Heathen has arrived\n no of remanining Prudes: %d\n no of Heathens: %d\n", prude_counter, heathens_counter);

    pthread_mutex_unlock(&_mutex);

    sleep(1);

   //Control the time between prudes and heathens. Prevents an individual from requesting the mutex without the faction turn
    sem_wait(&heathens_turn);

    pthread_mutex_lock(&_mutex);
    heathens_counter--;

    if (!heathens_counter)          
        if (prude_counter)         
            sem_post(&prudes_turn); 

    if (prude_counter > heathens_counter)
    {                           
        sem_post(&prudes_turn); 
    }
    else
    {
        sem_post(&heathens_turn);
    } //If not, leave the execution of the heathens turn


    printf("\nHeatean leaves the critical zone\n no of remaining prudes %d \n %d no of remaining Heathens\n", prude_counter, heathens_counter);

    pthread_mutex_unlock(&_mutex);
    pthread_exit(0);
}

int main()
{
    int nHeathens, nPrudes, start;
    pthread_t *_tidPrudes;
    pthread_t *_tidHeathens;

    printf("Enter the number of Heathens(at least 1)\n");
    scanf("%d", &nHeathens);
    if (nHeathens < 1)
        nHeathens = 20;

    printf("Enter the number of Prudes(at least 1)\n");
    scanf("%d", &nPrudes);
    if (nPrudes < 1)
        nPrudes = 20;

    //if there are more hesthens than prudes  the give the lock to heathens  else to prudes first
    if (nHeathens > nPrudes)
        start = 1; 
    else
        start = 0;

    //create threads for heathens and prudes
    _tidHeathens = (pthread_t *)malloc(nHeathens * sizeof(pthread_t)); 
    _tidPrudes = (pthread_t *)malloc(nPrudes * sizeof(pthread_t));     

    //validation of the created semaphores
    if (sem_init(&heathens_turn, 0, start) < 0)
    {
        exit(1);
    }

    if (sem_init(&prudes_turn, 0, !start) < 0)
    {
        exit(1);
    }

    //Create specifed amount of threads for heathens and prude
    if (nHeathens >= nPrudes)
    {
        //thread init
        for (int i = 0; i < nHeathens; i++)
        {
            pthread_create(&_tidHeathens[i], NULL, &heathens, NULL);
            if (i < nPrudes)
            {
                pthread_create(&_tidPrudes[i], NULL, &prudes, NULL);
            }
        }

        //join all threads once execution is over
        for (int i = 0; i < nHeathens; i++)
        {
            pthread_join(_tidHeathens[i], NULL);
            if (i < nPrudes)
            {
                pthread_join(_tidPrudes[i], NULL);
            }
        }
    }
    else
    {
        //threads init
        for (int i = 0; i < nPrudes; i++)
        {
            pthread_create(&_tidPrudes[i], NULL, &prudes, NULL);
            if (i < nHeathens)
            {
                pthread_create(&_tidHeathens[i], NULL, &heathens, NULL);
            }
        }


        for (int i = 0; i < nPrudes; i++)
        {
            pthread_join(_tidPrudes[i], NULL);
            if (i < nHeathens)
            {
                pthread_join(_tidHeathens[i], NULL);
            }
        }
    }
    return 0;
}