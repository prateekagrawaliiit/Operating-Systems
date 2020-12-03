/*
	RoomSearch1.c - One implementation of a potential solution to the 'Room Search' problem described as follows:
	
	The following synchronization constraints apply to students and the Dean of Students:
	1. Any number of students can be in a room at the same time.
	
	2. The Dean of Students can only enter a room if there are no students in the room (to conduct a search) or 
	if there are more than 50 students in the room (to break up the party).
	
	3. While the Dean of Students is in the room, no additional students may
	enter, but students may leave.
	
	4. The Dean of Students may not leave the room until all students have left.
	
	5. There is only one Dean of Students, so you do not have to enforce exclusion
	among multiple deans.
	
	The students and the dean are represented by different threads each with their own unique attributes and behaviour.
	Students have an ID, arrival time in seconds, and duration in seconds. Attributes for student threads must be provided in
	a file on the command line with the number of students to simulate on the first line and the attributes for each student
	on subsequent lines separated by commas. For example:
	
	3
	1,4,5
	2,8,2
	3,4,7
	
	This specifies 3 students to simulate. First has ID=1, arrival=4, duration=5.
											Second has ID=2, arrival=8, duration=2.
											Third has ID=3, arrival=4, duration=7.

	In addition to the input file a nonzero positive integer must be provided which will be the interval at which the dean
	comes to inspect the room. Invoke as: RoomSearch1 <student_file> <dean_interval_int>

	Author: Justin Underhay	
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


//Represents each student 
struct student {
	int ID;
	int arrival;
	int duration;
	double idle;
};


//Locks and conditions
pthread_mutex_t roomLock;
pthread_cond_t roomCond;
pthread_cond_t dCond;

bool dean_in;
int size;
int dean_interval;



/*
Student thread entry point. Students wait if the dean is in and enter when he leaves.
Students in the room with the dean exit normally but the last one to exit signals to the dean
that all students have left allowing him to leave.
*/
void *student_entry(void *info) {
	struct student *sThread = (struct student*) info;
	sleep(sThread->arrival);
	
	clock_t start = clock();
	pthread_mutex_lock(&roomLock);
	
	printf("Student %d has arrived\n",sThread->ID);
	
	if (dean_in)
		pthread_cond_wait(&roomCond, &roomLock);
	
	sThread->idle = (double) (clock() - start);
	printf("Student %d is entering room\n",sThread->ID);
	
	size++;
	pthread_mutex_unlock(&roomLock);
	
	sleep(sThread->duration);
	
	pthread_mutex_lock(&roomLock);
	size--;
	
	printf("Student %d has left\n",sThread->ID);
	
	if (size == 0)
		pthread_cond_signal(&dCond);
	
	pthread_mutex_unlock(&roomLock);
}


/*
Dean thread entry point. The dean runs in an infinite loop sleep for the interval time and then conducting an inspection
of the room. If there are no or above a certain amount of students in the room the dean will enter and wait there until all
students have left.
*/
void *dean_entry(void *info) {
	while (true) {
		sleep(dean_interval);
	
		pthread_mutex_lock(&roomLock);
	
		//The (size > (int)) condition can be changed to allow the dean entry when there are more than (int) students.
		//Right now that int is 50.
		if (size == 0 || size > 3) {
			dean_in = true;
			printf("Dean is entering room\n");
			
			if (size != 0)
				pthread_cond_wait(&dCond, &roomLock);
			
			dean_in = false;
			printf("Dean is leaving room\n");
			pthread_cond_broadcast(&roomCond);
		}

		pthread_mutex_unlock(&roomLock);
	}	
}


/*
Main function which reads file input, sets thread attributes and starts each thread then waits for each to terminate.
*/
int main(int argc, char *argv[]) {
	
	//Check for proper program invocation
	if (argc != 3) {
		printf("Invalid invocation, usage: RoomSearch1 <file-name> <integer>");
		exit(EXIT_FAILURE);
	}		
	
	dean_interval = atoi(argv[2]);
	
	if (dean_interval < 1) {
		printf("Invalid dean interval specified, must be greater than 0");
		exit(EXIT_FAILURE);
	}	
	
	//Initialize lock and conditions
	if (pthread_mutex_init(&roomLock, NULL) != 0) {
		printf("Mutex lock creation failure, exiting...");
		exit(EXIT_FAILURE);
	}	
	
	if (pthread_cond_init(&roomCond, NULL) != 0 || pthread_cond_init(&dCond, NULL) != 0) {
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
	
	size = 0;
	dean_in = false;
	
	pthread_t tID[numThreads];
	pthread_t deanThread;
	
	struct student *threads[numThreads];
	
	int x;
	
	for (x=0; x<numThreads; x++) {
		threads[x] = (struct student*)malloc(sizeof(struct student));
		
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
		
		threads[x]->arrival = atoi(conv);
		
		while (*conv != '\0')
			conv++;
		conv++;
		
		threads[x]->duration = atoi(conv);
	}	
	
	fclose(fp);
	
	
	if (pthread_create(&deanThread,NULL,dean_entry,NULL) != 0) {
		printf("Error in thread creation, quitting...");
		exit(EXIT_FAILURE);
	}		
	
	
	for (x=0; x<numThreads; x++) {
		if (pthread_create(&tID[x],NULL,student_entry,threads[x]) != 0) {
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