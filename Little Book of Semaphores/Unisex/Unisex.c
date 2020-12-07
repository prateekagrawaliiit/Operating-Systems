/*
	Unisex.c - The 2nd implementation of a potential solution to the 'Unisex Bathroom' problem.
	This implementation is a more fair approach that does a better job of avoiding starvation by allowing
	access to no more than 3 threads of the same gender sequentially. 3 Male threads will be allowed in then if there
	are female threads waiting 3 of them are allowed in regardless of whether there are more male threads waiting.
	Hence a 3 male, 3 female, 3 male... pattern is established. Of course if the bathroom is open to one gender 
	but less than 3 of that gender have come and gone then the other gender is allowed access.

	As with Unisex.java an input file must be specified containing the number of threads and their attributes at execution 
	with commas separating each value.

	Author: Justin Underhay	
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


//The threadStruct will hold the attributes for each thread and the pointer to the next thread in the queue
struct threadStruct {
	int ID;
	char gender;
	int arrivalTime;
	int duration;
	double idle;
};


//Locks and conditions
pthread_mutex_t doorLock;
pthread_cond_t mEntryCond, fEntryCond;


int size;
int num_done;
char turn;
bool wait;


/*
Thread entry point. As stated at top, the program tries to serve 3 males then 3 females and so on to avoid starvation
to one gender if the other has claimed the bathroom and has an excessively long lineup.
*/
void *thread_entry(void *info) {
	
	struct threadStruct *newThread = (struct threadStruct*)info;

	sleep(newThread->arrivalTime);
	clock_t start = clock();
	
	pthread_mutex_lock(&doorLock);
	
	if (newThread->gender == 'M') {

		while ((turn == 'F' || wait) && size != 0)
			pthread_cond_wait(&mEntryCond, &doorLock);
		
		newThread->idle = (double) (clock() - start);
		printf("Male thread %d entering bathroom\n",newThread->ID);
		
		if (size == 0) {
			num_done = 0;
			turn = 'M';
		}	
		
		size++;
		num_done++;
		
		if (num_done == 3)
			wait = true;
		
		
		pthread_mutex_unlock(&doorLock);
		
	} else {

		while ((turn == 'M' || wait) && size != 0)
			pthread_cond_wait(&fEntryCond, &doorLock);
		
		newThread->idle = (double) (clock() - start);
		printf("Female thread %d entering bathroom\n",newThread->ID);
		
		if (size == 0) {
			num_done = 0;
			turn = 'F';
		}	
		
		size++;
		num_done++;
		
		if (num_done == 3)
			wait = true;
		
		
		pthread_mutex_unlock(&doorLock);

	}
		
	sleep(newThread->duration);	//Represents duration in the critical section

	pthread_mutex_lock(&doorLock);
	size--;
	num_done--;
	
	if (newThread->gender == 'M') {
		
		if (wait && size == 0) {
			wait = false;
			turn = 'F';
			pthread_cond_broadcast(&fEntryCond);
		} else if (size == 0) {
			turn = 'F';
			pthread_cond_broadcast(&fEntryCond);
		}	
				
		
		printf("Male thread %d leaving bathroom\n",newThread->ID);
		
	} else {
		
		if (wait && size == 0) {
			wait = false;
			turn = 'M';
			pthread_cond_broadcast(&mEntryCond);
		} else if (size == 0) {
			turn = 'M';
			pthread_cond_broadcast(&mEntryCond);
		}
		
		printf("Female thread %d leaving bathroom\n",newThread->ID);

	}	

	pthread_mutex_unlock(&doorLock);
	
}	


/*
Main function which reads file input, sets thread attributes and starts each thread then waits for each to terminate.
*/
int main(int argc, char *argv[]) {
	
	//Check for proper program invocation
	if (argc != 2) {
		printf("Invalid invocation, usage: Unisex <file-name>");
		exit(EXIT_FAILURE);
	}	
	
	//Initialize lock and conditions
	if (pthread_mutex_init(&doorLock, NULL) != 0) {
		printf("Mutex lock creation failure, exiting...");
		exit(EXIT_FAILURE);
	}	
	
	if (pthread_cond_init(&mEntryCond, NULL) != 0 || pthread_cond_init(&fEntryCond, NULL) != 0) {
		printf("Condition variable creation failure, exiting...");
		exit(EXIT_FAILURE);
	}

	
	//Following sections set global variables, read the input file, assign thread attributes, create and then wait for threads to terminate
	size = 0;
	num_done = 0;
	turn = 'M';
	wait = false;
	
	char buffer[15];
	char *pos = buffer;
	char *conv = buffer;
	
	FILE *fp = fopen(argv[1], "r");
	
	fgets(buffer, 5, fp);
	
	int numThreads = atoi(buffer);
	
	if (numThreads < 0) {
		printf("Illegal number of threads specified, must be positive.");
		exit(EXIT_FAILURE);
	}	
	
	pthread_t tID[numThreads];
	
	struct threadStruct *threads[numThreads];
	
	int x;
	
	for (x=0; x<numThreads; x++) {
		threads[x] = (struct threadStruct*)malloc(sizeof(struct threadStruct));
		
		fgets(buffer, 15, fp);
		
		pos = buffer;
		conv = buffer;
	
		while (*pos != ',')
			pos++;
		*pos = '\0';

		threads[x]->ID = atoi(conv);
		
		while (*pos != ',')
			pos++;
		*pos = '\0';
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		threads[x]->gender = toascii(*conv);
		
		while (*pos != ',')
			pos++;
		*pos = '\0';
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		threads[x]->arrivalTime = atoi(conv);
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		threads[x]->duration = atoi(conv);
	}	
	
	fclose(fp);
	
	
	for (x=0; x<numThreads; x++) {
		if (pthread_create(&tID[x],NULL,thread_entry,threads[x]) != 0) {
			printf("Thread creation error, quitting...");
			exit(EXIT_FAILURE);
		}	
	}	
	
	for (x=0; x<numThreads; x++) {
		if (pthread_join(tID[x],NULL) != 0) {
				printf("Error in thread wait termination, ending simulation\n");
				exit(EXIT_FAILURE);	
		}
	}
	
	double sum = 0;
	for (x=0; x<numThreads; x++) {
		printf("\nThread %d idle time: %f\n",threads[x]->ID,threads[x]->idle/CLOCKS_PER_SEC);
		sum += threads[x]->idle/CLOCKS_PER_SEC;
	}

	printf("\nAverage idle time: %f",sum/numThreads);	

	return 0;
}	