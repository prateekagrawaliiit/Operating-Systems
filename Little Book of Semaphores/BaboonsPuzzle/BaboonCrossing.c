/*
 * main.c
 *
 *  Created on: Nov 11, 2012
 *      Author: srikanra
 */

//INCLUDES AND MACROS
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAX_BABOONS_ON_ROPE 2

//STRUCTURES AND ENUMS
typedef enum dir
{
	DIRECTION_1 = 0,
	DIRECTION_2,
	DIRECTION_MAX
}enumDir_t;

typedef enum error_t
{
	ERROR = -1,
	SUCCESS,
	ERROR_MAX
}error_t;

typedef struct baboon
{
    enumDir_t dirFlag;
    int hasCrossed;
    int bbIdx;
    int sleep;
}baboon_t;

//GLOBALS

enumDir_t currentDirection;

int baboonCountInCurrentDirection;

typedef enum semIdx
{
	SEM_CNT_ROPE_MAX_CNT = 0,
	SEM_BIN_PROTECT_COUNT,
	SEM_BIN_Q_PROTECT,
	SEM_TYPE_MAX
}semIdx_t;
char* semNames[] = {"semRopeMaxCount","semProtectCount","semQueueProtect"};
sem_t* semRopeMaxCount;
sem_t* semProtectCount;
sem_t* semQueueProtect;

int *queue;
int currIdx;
int nextBaboon;


//FUNCTIONS
error_t SetUpRope(void);
error_t SetUpBaboon(baboon_t* baboon, int bbIdx);
error_t SetTheBaboonsLoose(int numBaboons);
void ResetRope(void);
void* Baboon(void*args);


error_t SetUpRope(void)
{
	error_t err = ERROR;
	if((semRopeMaxCount = sem_open(semNames[SEM_CNT_ROPE_MAX_CNT],O_CREAT,S_IWUSR|S_IWGRP|S_IWOTH,MAX_BABOONS_ON_ROPE)) != NULL)
	{
		if((semProtectCount = sem_open(semNames[SEM_BIN_PROTECT_COUNT],O_CREAT,S_IWUSR|S_IWGRP|S_IWOTH,MAX_BABOONS_ON_ROPE)) != NULL)
		{
			if((semQueueProtect = sem_open(semNames[SEM_BIN_Q_PROTECT],O_CREAT,S_IWUSR|S_IWGRP|S_IWOTH,MAX_BABOONS_ON_ROPE)) != NULL)
			{
				err = SUCCESS;
			}
		}
	}
	printf("SetUpRope : errno %d \n",errno);

	return err;
}

void ResetRope(void)
{
	if(semRopeMaxCount)
		sem_close(semRopeMaxCount);
	if(semProtectCount)
		sem_close(semProtectCount);
	if(semQueueProtect)
		sem_close(semQueueProtect);
}

error_t SetUpBaboon(baboon_t* baboon, int bbIdx)
{
	error_t err = ERROR;

	//printf("SetUpBaboon :%d \n", bbIdx);

	baboon->dirFlag = rand()%2;
	baboon->hasCrossed = 0;
	baboon->bbIdx = bbIdx;
	baboon->sleep = rand()%5;

	err = SUCCESS;

	return err;
}

error_t SetTheBaboonsLoose(int numBaboons)
{
	error_t err = ERROR;
	pthread_t* threads;
	baboon_t* baboonArgs;
	int bbIdx;

	if(SetUpRope() == ERROR)
	{
		printf("SetTheBaboonsLoose: SetUpRope Failed \n");
		goto ERR;
	}

	//Pthreads
	threads = (pthread_t*)malloc(numBaboons*sizeof(pthread_t));
	if(threads == NULL)
	{
		printf("SetTheBaboonsLoose: PTHREAD_T MALLOC FAILED \n");
		goto ERR;
	}

    //Pthread Args
	baboonArgs = (baboon_t*)malloc(numBaboons*sizeof(baboon_t));
    if(baboonArgs == NULL)
    {
		printf("SetTheBaboonsLoose: BABOON_T MALLOC FAILED \n");
		goto ERR;
    }

    queue = (int*)malloc(sizeof(int)*numBaboons);
    if(queue == NULL)
    {
		printf("SetTheBaboonsLoose: QUEUE MALLOC FAILED \n");
		goto ERR;
    }
    currIdx = 0;
    nextBaboon = 0;

    //Setup the thread args for each baboon and start the thread
    for(bbIdx = 0; bbIdx < numBaboons; bbIdx++)
    {
    	SetUpBaboon(&baboonArgs[bbIdx],bbIdx);
    	if(pthread_create(&threads[bbIdx],NULL,Baboon,(void*)&baboonArgs[bbIdx]) != 0)
    	{
    		goto ERR;
    	}
    }

    //Join all the baboon threads to the main thread
    for(bbIdx = 0; bbIdx < numBaboons; bbIdx++)
    {
    	if(pthread_join(threads[bbIdx],NULL) != 0)
    	{
    		printf("SetTheBaboonsLoose: Baboon %d ran away \n",bbIdx);
    	}
    }

ERR:
    if(threads)
	{
		free(threads);
	}
    if(baboonArgs)
    {
    	free(baboonArgs);
    }
    if(queue)
    {
    	free(queue);
    }
    ResetRope();
    return err;
}

void updateQueue(baboon_t* bbArgs)
{
	sem_wait(semQueueProtect);
	if(0 == currIdx)
		currentDirection = bbArgs->dirFlag;
	queue[currIdx++] = bbArgs->bbIdx;
	sem_post(semQueueProtect);
}


void* Baboon(void *args)
{
	char log[128];
	baboon_t *baboon = (baboon_t *)args;

	if(baboon)
	{
		//printf("Baboon %d will cross in direction %d \n", baboon->bbIdx, baboon->dirFlag);
		updateQueue(baboon);
		while(baboon->hasCrossed != 1)
		{
			if(queue[nextBaboon] == baboon->bbIdx)
			{
					//Update the count of baboons who came in first
					if(baboon->dirFlag == currentDirection)
					{
						sem_wait(semProtectCount);
						baboonCountInCurrentDirection++;
						nextBaboon++; //Let the next baboon in queue,try to cross.
						sem_post(semProtectCount);
					}
					else
					{
						continue; // Wait for the direction to chance to let baboon in.
					}

					//Allow a fixed number on the rope and start crossing and simulate crossing by sleeping
					sem_wait(semRopeMaxCount);
					printf("Baboon %d crossing in direction %d, %d baboons left in this direction \n",baboon->bbIdx, baboon->dirFlag,baboonCountInCurrentDirection);
					sleep(baboon->sleep);
					//printf("Baboon %d crossed in direction %d \n", baboon->bbIdx, baboon->dirFlag);
					sem_wait(semProtectCount); //Get off the rope
					baboonCountInCurrentDirection--;
					if(baboonCountInCurrentDirection == 0)
					{
						currentDirection = ((currentDirection == DIRECTION_1)?DIRECTION_2:DIRECTION_1);
						printf("Change of direction to %d \n",currentDirection);
					}
					sem_post(semProtectCount);
					sem_post(semRopeMaxCount);

					baboon->hasCrossed = 1;
			}
		}
	}
	else
	{
		printf(log,"BABOON : Input args NULL \n");
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	int lBaboonCount = 0;

	printf("Main Starts!!!\n");

	if(argc != 2)
	{
		printf("Please enter the correct numbe of arguments \nExample : ./<Exec> <No of Baboons> \n");
		return -1;
	}
	else
	{
		lBaboonCount = atoi(argv[1]);
		printf("The number of baboons entered: %d \n", lBaboonCount);
		SetTheBaboonsLoose(lBaboonCount);
	}
	printf("Main Ends !!\n");
    pthread_exit(NULL);
	return 0;
}
