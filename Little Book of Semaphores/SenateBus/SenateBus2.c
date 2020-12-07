/*
	SenateBus2.c - The second implementation of a potential solution to the 'Senate Bus' problem.
	The style of solution is identical to SenateBus1.java but implemented in a different language.
	
	As with SenateBus1.java two files must be provided on the command line at execution containing
	thread information. Invoke as: SenateBus2 <passenger-file> <bus-file>
	
	Author: Justin Underhay	
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


//A representation of each passenger
struct passenger {
	int ID;
	int arrival;
	double idle;
};

//A representation of each bus
struct bus {
	int ID;
	int arrival;
};	


//Locks and conditions
pthread_mutex_t entryLock;
pthread_cond_t entry;
pthread_cond_t wait;
pthread_cond_t busWait;
pthread_cond_t nextBus;

int ticket;
bool boarding;



/*
Passenger thread entry point. Like with the first implementation passenger receive a ticket number upon arrival
which determines whether they will board the next bus. When a bus arrives it will signal all waiting passengers to
board and the last one to do so will signal the bus to depart. The departing bus will then signal threads waiting
beyond the 50th ticket to start getting in line.
*/
void *passenger_entry(void *info) {
	struct passenger *pThread = (struct passenger*) info;
	sleep(pThread->arrival);
	
	clock_t start = clock();
	printf("Passenger %d has arrived\n",pThread->ID);
	
	pthread_mutex_lock(&entryLock);
	
	if (boarding || ticket > 49)
		pthread_cond_wait(&entry, &entryLock);
	
	ticket++;
	
	while (!boarding)
		pthread_cond_wait(&wait, &entryLock);
	
	pThread->idle = (double) (clock() - start);
	printf("Passenger %d now boarding\n",pThread->ID);
	
	ticket--;
	
	if (ticket == 0) 
		pthread_cond_signal(&busWait);	
	
	pthread_mutex_unlock(&entryLock);
}	


/*
Bus thread entry point. An arriving bus will signal to passenger threads to board and wait until the last 
passenger to board signals it. Of course the bus leaves immediately if no passengers are waiting.	
*/
void *bus_entry(void *info) {
	struct bus *bThread = (struct bus*) info;
	sleep(bThread->arrival);
	
	printf("Bus %d has arrived\n",bThread->ID);
	
	pthread_mutex_lock(&entryLock);
	
	if (boarding)
		pthread_cond_wait(&nextBus, &entryLock);
	
	if (ticket == 0) {
		pthread_mutex_unlock(&entryLock);
		printf("Bus %d leaving with no passengers\n",bThread->ID);
		exit(1);
	}

	boarding = true;

	pthread_cond_broadcast(&wait);
	pthread_cond_wait(&busWait, &entryLock);

	ticket = 0;
	boarding = false;
	
	printf("Bus %d now leaving\n",bThread->ID);

	pthread_cond_signal(&nextBus);	
	pthread_mutex_unlock(&entryLock);
}


/*
Main function which reads file input, sets thread attributes and starts each thread then waits for each to terminate.
*/
int main(int argc, char *argv[]) {
	
	//Check for proper program invocation
	if (argc != 3) {
		printf("Invalid invocation, usage: SenateBus2 <passenger-file> <bus-file>");
		exit(EXIT_FAILURE);
	}
	
	//Initialize lock and conditions
	if (pthread_mutex_init(&entryLock, NULL) != 0) {
		printf("Mutex lock creation failure, exiting...");
		exit(EXIT_FAILURE);
	}	
	
	if (pthread_cond_init(&entry, NULL) != 0 || pthread_cond_init(&wait, NULL) != 0 || pthread_cond_init(&busWait, NULL) != 0 || pthread_cond_init(&nextBus, NULL) != 0) {
		printf("Condition variable creation failure, exiting...");
		exit(EXIT_FAILURE);
	}	
	
	
	//Following sections set global variables, read the input file, assign thread attributes, create and then wait for threads to terminate
	ticket = 0;
	boarding = false;
	
	char buffer[15];
	char *pos = buffer;
	char *conv = buffer;
	
	FILE *fp = fopen(argv[1], "r");
	
	fgets(buffer, 5, fp);
	
	int PnumThreads = atoi(buffer);
	
	if (PnumThreads < 0) {
		printf("Illegal number of threads specified, must be positive.");
		exit(EXIT_FAILURE);
	}	
	
	pthread_t pID[PnumThreads];
	
	struct passenger *Pthreads[PnumThreads];
	
	int x;
	
	for (x=0; x<PnumThreads; x++) {
		Pthreads[x] = (struct passenger*)malloc(sizeof(struct passenger));
		
		fgets(buffer, 15, fp);
		
		pos = buffer;
		conv = buffer;
	
		while (*pos != ',')
			pos++;
		*pos = '\0';

		Pthreads[x]->ID = atoi(conv);
		
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		Pthreads[x]->arrival = atoi(conv);
		
	}	
	
	fclose(fp);
	
	
	fp = fopen(argv[2], "r");
	
	fgets(buffer, 5, fp);
	
	int BnumThreads = atoi(buffer);
	
	if (BnumThreads < 0) {
		printf("Illegal number of threads specified, must be positive.");
		exit(EXIT_FAILURE);
	}
	
	pthread_t bID[BnumThreads];
	
	struct bus *Bthreads[BnumThreads];
	
	for (x=0; x<BnumThreads; x++) {
		Bthreads[x] = (struct bus*)malloc(sizeof(struct bus));
		
		fgets(buffer, 15, fp);
		
		pos = buffer;
		conv = buffer;
	
		while (*pos != ',')
			pos++;
		*pos = '\0';

		Bthreads[x]->ID = atoi(conv);
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		Bthreads[x]->arrival = atoi(conv);
	}	
	
	fclose(fp);
	
	
	for (x=0; x<PnumThreads; x++) {
		if (pthread_create(&pID[x],NULL,passenger_entry,Pthreads[x]) != 0) {
			printf("Thread creation error, quitting...");
			exit(EXIT_FAILURE);
		}	
	}

	for (x=0; x<BnumThreads; x++) {
		if (pthread_create(&bID[x],NULL,bus_entry,Bthreads[x]) != 0) {
			printf("Thread creation error, quitting...");
			exit(EXIT_FAILURE);
		}	
	}	
	
	for (x=0; x<PnumThreads; x++) {
		if (pthread_join(pID[x],NULL) != 0) {
			printf("Error in thread wait termination, ending simulation\n");
			exit(EXIT_FAILURE);	
		}
	}
	
	for (x=0; x<BnumThreads; x++) {
		if (pthread_join(bID[x],NULL) != 0) {
			printf("Error in thread wait termination, ending simulation\n");
			exit(EXIT_FAILURE);	
		}
	}
	
	double sum = 0;
	for (x=0; x<PnumThreads; x++) {
		printf("\nPassenger %d idle time: %f\n",Pthreads[x]->ID,Pthreads[x]->idle/CLOCKS_PER_SEC);
		sum += Pthreads[x]->idle/CLOCKS_PER_SEC;
	}	
	
	printf("\n\nAverage idle time: %f",sum/PnumThreads);

	return 0;
}
