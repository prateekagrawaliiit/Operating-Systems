#include "main.h"
#include <pthread.h>

#define CAPACITY 10
#define SAVAGES 5

volatile int servings = CAPACITY;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t emptyCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t fullCond = PTHREAD_COND_INITIALIZER;


int main()
{                    
    pthread_t cookThread;
    pthread_create(&cookThread, NULL, cookerThread, NULL);
    
    pthread_t savageThreads[SAVAGES];
    int savageIds[SAVAGES];

    for (int i = 0; i < SAVAGES; i++)
    {
        savageIds[i] = i + 1;
        pthread_create(&savageThreads[i], NULL, savageThread, &savageIds[i]);
    }

    // Join the cook thread so that the execution won't finish until the thread does
    pthread_join(cookThread, NULL);
    return 0;
}

void *cookerThread()
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while (servings != 0)
        {
            pthread_cond_wait(&emptyCond, &mutex);
        }
        
        servings = CAPACITY;
        printf("[Cooker] Filled the pot\n");
        pthread_cond_signal(&fullCond);
        pthread_mutex_unlock(&mutex);
    }
}

void *savageThread(void* savageIdPtr)
{
    int savageId = *(int*)savageIdPtr;
    while(1)
    {
        getServingFromPot(savageId);
        eat(savageId);
    }
}

void getServingFromPot(int savageId)
{
    pthread_mutex_lock(&mutex);
    while (servings == 0)
    {
        pthread_cond_signal(&emptyCond);
        pthread_cond_wait(&fullCond, &mutex);
    }

    servings--;
    printf("[Savage] %i took a portion.\n", savageId);
    pthread_mutex_unlock(&mutex);
}

void eat(int savageId)
{
    // Uses the savage id as a seed for the random number generator
    int sleepTime = getRandom(savageId, 10);
    waitFor(sleepTime);
}