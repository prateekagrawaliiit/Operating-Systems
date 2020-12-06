#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

int eating = 0, waiting = 0;
bool must_wait = false;
sem_t block;                                                            
sem_t mutex;

void * sushiBar(void * threadID)
{
usleep(rand() % 1000);

sem_wait(&mutex);
if(must_wait){
    printf("Waits: %d seats are available. %d other people waiting.\n", 5-eating, waiting);
    waiting++;
    sem_post(&mutex);
    sem_wait(&block);
    waiting--;
}
if(eating == 4)
    printf("Last seat is taken.\n");
else{
    printf("%d seats available. Sits and eats sushi.\n", 5-eating);
}

eating++;
must_wait = (eating == 5);

if(waiting && (!must_wait))
    sem_post(&block);
else
    sem_post(&mutex);

usleep((rand() % 901) + 100);
sem_wait(&mutex);
eating--;
printf("Customer leaves: %d seats are available.\n", 5-eating);
if (eating == 0)
    must_wait = false;
if ( waiting && (!must_wait))
    sem_post(&block);
else
    sem_post(&mutex);
return 0;
}

int main(){
 int n=10,i=0,retVal=0;
 pthread_t *thread;

sem_init(&mutex, 0, 1);
sem_init(&block, 0, 0);

thread = (pthread_t *) malloc (n*sizeof(pthread_t));

for (i=0; i<n; i++){
  retVal = pthread_create(&thread[i], NULL, sushiBar, (void*)&i);
  if (retVal != 0){
    exit(EXIT_FAILURE);        
   }
}

for(i=0; i<n; i++){
    retVal = pthread_join(thread[i],NULL);
        if(retVal != 0){
           exit(EXIT_FAILURE);        
        }
 }

return 0;
}