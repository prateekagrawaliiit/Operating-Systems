/*
	SID1.c - One implementation of a potential solution to the 'Search-Insert-Delete' problem.
	3 types of threads share access to a singly linked list: Searchers, Inserters, and Deleters.
	Searchers merely examine the list hence they can execute concurrently with each other. Inserters add
	new items to the end of the list; insertions must be mutually exclusive to preclude two inserters from 
	inserting new items at about the same time. However, one insert can proceed in parallel with any number of searches. 
	Finally, deleters remove items from anywhere in the list. At most one deleter process can access the list at
	a time, and deletion must also be mutually exclusive with searches and insertions.
	
	The problem specifies a single linked list but for reasons of ease a doubly linked list is used. The focus is on
	ensuring synchronization after all.	
	
	The number of threads along with their attributes must be specified by a file provided on the command line at execution.
	The file should have on the first line the number of threads to simulate and on subsequent lines each thread's ID, type
	as a single char of 'S', 'I', or 'D', and value to perform its action on separated by commas. For example:
	
	3
	1,S,4
	2,I,23
	3,D,18
	
	This specifies 3 threads: The 1st is a searcher for 4, the 2nd a inserter of 23, and the 3rd a deleter of 18.
	
	Author: Justin Underhay

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


//Head of the linked list
struct link *HEAD;

//Link structs hold make up the linked list
struct link {
	int data;
	struct link *prev;
	struct link *next;
};

//The threadStruct will hold the attributes for each thread and the pointer to the next thread in the queue
struct threadStruct {
	char type;
	int val;
	int ID;
	double idle;
};	
	
//The critical struct that should only be accesses by one thread at a time	
struct acc_status {
	bool del_in;
	bool ins_in;
	int size;
	int del_wait_amt;
};

struct acc_status listStat;

//Locks and conditions
pthread_mutex_t statusLock;
pthread_cond_t delCond, inCond, sCond;	


/*
Searchers use this function to find values in the list
*/
struct link* search_list(int target) {
	struct link *pnt = HEAD;
	while (pnt != NULL && pnt->data != target)
		pnt = pnt->next;
	return pnt;
}	


/*
Inserters use this function to insert into the list
*/
void insert_list(int entry) {
	struct link *pnt = HEAD;
	if (HEAD == NULL) {
		HEAD = 	(struct link*)malloc(sizeof(struct link));
		HEAD->data = entry;
		HEAD->next = NULL;
		HEAD->prev = NULL;
	} else {
		while (pnt->next != NULL)
			pnt = pnt->next;
		pnt->next = (struct link*)malloc(sizeof(struct link));
		pnt->next->prev = pnt;
		pnt = pnt->next;
		pnt->data = entry;
		pnt->next = NULL;
	}	
}


/*
Deleters use this function to delete from the list
*/
bool delete_from(int target) {
	struct link *pnt = search_list(target);
	if (pnt == NULL)
		return false;
	
	if (pnt == HEAD)
		free(pnt);
	else {
		pnt->prev->next = pnt->next;
		free(pnt);
	}
	
	return true;
}	


/*
Threads begin execution here. Priority is given to deleters. If there are deleters waiting to enter, searchers and inserters
do not proceed and only enter when no deleter is waiting. Searchers and inserters abide by their own rules.
*/
void *thread_entry(void *info) {
	clock_t start = clock();
	
	struct threadStruct *newThread = (struct threadStruct*)info;
	
	if (newThread->type == 'D') {
		pthread_mutex_lock(&statusLock);
		listStat.del_wait_amt++;
		
		if (listStat.size > 0)
			pthread_cond_wait(&delCond, &statusLock);
		
		listStat.del_in = true;
		listStat.size++;
		listStat.del_wait_amt--;
		
		pthread_mutex_unlock(&statusLock);
		
	} else if (newThread->type == 'S') {
		pthread_mutex_lock(&statusLock);
		
		if (listStat.del_in || listStat.del_wait_amt >= 1)
			pthread_cond_wait(&sCond, &statusLock);
		
		listStat.size++;
		pthread_mutex_unlock(&statusLock);
		
	} else {
		pthread_mutex_lock(&statusLock);
		
		if (listStat.del_in || listStat.del_wait_amt >= 1 || listStat.ins_in)
			pthread_cond_wait(&inCond, &statusLock);
		
		listStat.size++;
		listStat.ins_in = true;
		
		pthread_mutex_unlock(&statusLock);	
	}	
	
	//The start of the critical section
	struct link *s;
	switch (newThread->type) {
		case 'S':
			s = search_list(newThread->val);
			if (s != NULL)
				printf("\nSearcher %d found %d\n",newThread->ID,s->data);
			else
				printf("\nSearcher %d did not find %d\n",newThread->ID,newThread->val);
			break;
		case 'I':
			insert_list(newThread->val);
			printf("\nInserter %d inserted %d\n",newThread->ID,newThread->val);
			break;
		case 'D':
			if (delete_from(newThread->val))
				printf("\nDeleter %d removed %d\n",newThread->ID,newThread->val);
			else
				printf("\nDeleter %d did not find %d\n",newThread->ID,newThread->val);
	}		
	//End of the critical section
	
	pthread_mutex_lock(&statusLock);
	
	if (newThread->type == 'D')
		listStat.del_in = false;
	listStat.size--;
	
	if (listStat.del_wait_amt > 0 && listStat.size == 0)
		pthread_cond_signal(&delCond);
    else {
		
		if (newThread->type == 'I' || newThread->type == 'D') {
			listStat.ins_in= false;
			listStat.size--;
			pthread_cond_signal(&inCond);
		}
		pthread_cond_broadcast(&sCond);
	}	
	newThread->idle = (double) (clock() - start);
	pthread_mutex_unlock(&statusLock);
}	
	

	
/*
Main function which reads file input, sets thread attributes and starts each thread then waits for each to terminate.
*/
int main(int argc, char *argv[]) {
	
	//Check for proper program invocation
	if (argc != 2) {
		printf("Invalid invocation, usage: Search-Insert-Delete1 <file-name>");
		exit(EXIT_FAILURE);
	}
	
	//Initialize lock and conditions
	if (pthread_mutex_init(&statusLock, NULL) != 0) {
		printf("Mutex lock creation failure, exiting...");
		exit(EXIT_FAILURE);
	}	
	
	if (pthread_cond_init(&delCond, NULL) != 0 || pthread_cond_init(&inCond, NULL) != 0 || pthread_cond_init(&sCond, NULL) != 0) {
		printf("Condition variable creation failure, exiting...");
		exit(EXIT_FAILURE);
	}	
	
	
	//Following sections set global variables, read the input file, assign thread attributes, create and then wait for threads to terminate
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
	
	HEAD = NULL;
	
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
		
		threads[x]->type = toascii(*conv);
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		threads[x]->val = atoi(conv);
	}	
	
	fclose(fp);
	
	listStat.del_in = listStat.ins_in = false;
	listStat.del_wait_amt = listStat.size = 0;
	
	
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

	printf("\n\nAverage idle time: %f",sum/numThreads);	

	return 0;
}