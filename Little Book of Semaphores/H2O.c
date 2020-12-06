#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
sem_t smutex,oxyQueue,hydroQueue;

int oxygen=0,hydregen=0;

pthread_t oxyThread,hydroThread1,hydroThread2;

int bond(){
    static int i=0;
    i++;
    if((i%3)==0)
        printf("** Molecule no. %d created**\n\n",i/3);
    sleep(2);
    return(0);
}

void* oxyFn(void* arg){

    while(1){
        sem_wait(&smutex);

        oxygen+=1;

        if(hydregen>=2){
            sem_post(&hydroQueue);
            sem_post(&hydroQueue);
            hydregen-=2;
            sem_post(&oxyQueue);
            oxygen-=1;
        }
        else {
            sem_post(&smutex);
        }

        sem_wait(&oxyQueue);
        printf("Oxygen Bond\n");
        bond();

        sleep(3);
        sem_post(&smutex);
    }
}

void* hydroFn(void* arg){
    while(1){
        sem_wait(&smutex);

        hydregen+=1;

        if(hydregen>=2 && oxygen>=1){
            sem_post(&hydroQueue);
            sem_post(&hydroQueue);
            hydregen-=2;
            sem_post(&oxyQueue);
            oxygen-=1;
        }
        else{
            sem_post(&smutex);
        }

        sem_wait(&hydroQueue);

        printf("Hydrogen Bond\n");
        bond();
        sleep(3);
        }

}

int main(){

    if(sem_init(&smutex,0,1)==-1){
        perror("error initilalizing semaphore\n");
    }
    if(sem_init(&oxyQueue,0,0)==-1){
        perror("error initilalizing semaphore\n");
    }
    if(sem_init(&hydroQueue,0,0)==-1){
        perror("error initilalizing semaphore\n");
    }
    sleep(2);
    pthread_create(&oxyThread,0,oxyFn, NULL);
    pthread_create(&hydroThread1,0,hydroFn, NULL);
    pthread_create(&hydroThread2,0,hydroFn, NULL);

    for(;;);

}
