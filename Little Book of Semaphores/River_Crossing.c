//5.7.2 River crossing problem

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define MAX_PASSENGERS 100
//This struct stores the complete state at any time
struct State
{
  //how many people waiting to board:
  int hackersQueue;
  int serfsQueue;
  //how many people onboard
  int hackersOnboard;
  int serfsOnboard;
  int freeSpots;
  //how many passengers have crossed so far:
  int crossedHackers;
  int crossedSerfs;
  //attentionCalled  will be used to inform the threads if they need to leave
 //or if the boat is departing
  int attentionCalled;
  //passengers must leave the boat
  int leaveBoat;
  //boat is departing
  int depart;

} state;

void printState()
{
  printf("***********************\n");
  printf("*State:               *\n");
  printf("* %i  hackers queue   *\n",state.hackersQueue);
  printf("* %i  serfs queue     *\n",state.serfsQueue);
  printf("* %i  free spots      *\n",state.freeSpots);
  printf("* %i  serfs onboard   *\n",state.serfsOnboard);
  printf("* %i  hackers onboard *\n",state.hackersOnboard);
  printf("* %i  hackers crossed *\n",state.crossedHackers);
  printf("* %i  serfs crossed   *\n",state.crossedSerfs);
  printf("***********************\n");
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void rowBoat()
{
  printf("Boat is sailing \n");

  state.crossedHackers += state.hackersOnboard;
  state.crossedSerfs += state.serfsOnboard;
  state.serfsOnboard = 0;
  state.hackersOnboard = 0;
  state.freeSpots = 4;
}

bool hackerCanBoard()
{
  //check if hacker can safely board
  if ((state.hackersOnboard ==0 && state.serfsOnboard ==3)
      ||(state.hackersOnboard ==2 && state.serfsOnboard ==1)
    ||(state.freeSpots == 0))
        return false;

  //check if hacker can cause a deadlock
  if ((state.hackersOnboard ==2 && state.hackersQueue == 0)
    ||(state.hackersOnboard == 0 && state.hackersQueue == 1))
    {
        return false;
    }
  return true;
}

bool serfCanBoard()
{
  //check if serf can safely board
  if ((state.serfsOnboard ==0 && state.hackersOnboard ==3)
      ||(state.serfsOnboard ==2 && state.hackersOnboard ==1)
    ||(state.freeSpots == 0))
    {
        return false;
    }
    //check if hacker can cause a deadlock
    if ((state.serfsOnboard ==2 && state.serfsQueue == 1)
      ||(state.serfsOnboard == 0 && state.serfsQueue == 1))
      {
          return false;
      }
  return true;
}

void unboardEveryone()
{
  printf("Everyone unboarded \n");
  state.hackersQueue += state.hackersOnboard;
  state.serfsQueue += state.serfsOnboard;
  state.serfsOnboard = 0;
  state.hackersOnboard = 0;
  state.freeSpots = 4;
}

void boardHacker()
{
  printf("A hacker has just boarded \n");
  state.hackersQueue-=1;
  state.freeSpots-=1;
  state.hackersOnboard+=1;
}

void boardSerf()
{
  printf("A serf has just boarded \n");
  state.serfsQueue-=1;
  state.freeSpots-=1;
  state.serfsOnboard+=1;
}

bool canFillNewBoat()
{
  //if there are no possible combinations, job is done
  if(state.serfsQueue + state.serfsOnboard == 1 && state.hackersOnboard + state.hackersQueue <= 3 )
    return false;
  if(state.serfsQueue + state.serfsOnboard <= 3 && state.hackersOnboard + state.hackersQueue == 1 )
    return false;
  return true;
}

void *hackerThread()
{
  //sleep for up to 2s to add some randomness

    usleep(rand()%2000000);
    pthread_mutex_lock(&mutex);
    //thread does not continue until it can board
    while (!hackerCanBoard())
        pthread_cond_wait(&cond, &mutex);
    boardHacker();
    printState();
    //boat is full, it can leave. Last one to join the pool is the captain
    if(state.freeSpots == 0)
    {
      rowBoat();
      printState();
    }
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    //hacker thread has finished its work, it can modify the cond variable and
    //unlock the mutex

    return NULL;

}

void *serfThread()
{
  //sleep for up to 2s to add some randomness
  usleep(rand()%2000000);

  pthread_mutex_lock(&mutex);

  //thread does not continue until it can board
  while (!serfCanBoard())
      pthread_cond_wait(&cond, &mutex);
  boardSerf();
  printState();
  //boat is full, it can leave
  if(state.freeSpots == 0)
  {
    //last one to join calls row_boat, this way we make sure it is called
    //only once
    rowBoat();
    printState();
  }
  //serf thread has finished its work, it can modify the cond variable and
  //unlock the mutex
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  return NULL;
}



int main()
{

  int initialSerfsQueue = rand()%MAX_PASSENGERS;
  int initialHackersQueue = MAX_PASSENGERS - initialSerfsQueue;
  state.hackersQueue = initialSerfsQueue;
  state.serfsQueue = initialHackersQueue;
  state.hackersOnboard = 0;
  state.serfsOnboard = 0;
  state.freeSpots = 4;
  state.crossedHackers = 0;
  state.crossedSerfs = 0;

  pthread_t passengers[MAX_PASSENGERS];
  for(int i = 0 ; i < initialSerfsQueue; i++)
  {
    pthread_create(&passengers[i],NULL,hackerThread,NULL);
  }
  for(int i = state.hackersQueue; i < initialHackersQueue + initialSerfsQueue; i++)
  {
    pthread_create(&passengers[i],NULL,serfThread,NULL);
  }
  pthread_mutex_lock(&mutex);
  //finish program only we cannot send boats or if the last combination is
  //invalid 

  //do not continue while we can still have valid combinations
  while (canFillNewBoat())
      pthread_cond_wait(&cond, &mutex);
  pthread_mutex_unlock(&mutex);

  printf("All possible passengers have completed their trips \n");
  printf("***********************\n");
  printf("* %i  hackers crossed *\n",state.crossedHackers);
  printf("* %i  serfs crossed   *\n",state.crossedSerfs);
  printf("***********************\n");
  return(0);

}
