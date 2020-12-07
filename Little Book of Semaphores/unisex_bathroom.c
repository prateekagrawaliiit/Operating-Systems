#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>

#define MCOUNT 6		// number of male threads
#define FCOUNT 6		// number of female threads
#define RR_CAP 4		// maximum capacity of the restroom

#define RR_MAXSLEEP 10		// maximum time for thread to sleep
#define OTHER_SLEEP 15		// maximum time for thread to do other stuff

void *thread_fmain(void *);
void *thread_mmain(void *);
void man_enter();
void man_leave();
void woman_enter();
void woman_leave();
void use_rr(int);
void do_other_stuff(int);
int get_simple_tid(pthread_t, int);

int GENDER_CHECK;		// global variable to maintain gender check in the restroom
int value=RR_CAP;

pthread_t threadMIDs[MCOUNT];
pthread_t threadFIDs[FCOUNT];

//Defining semaphores, mutexes, etc here.
sem_t capacity_semaphore;
pthread_mutex_t semcheck= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t gendercheck= PTHREAD_MUTEX_INITIALIZER;


int main()
{
	pthread_attr_t attribs;
	int i,j;
	int tmids[MCOUNT];
    	int tfids[FCOUNT];
	pthread_t pthreadmids[MCOUNT];
	pthread_t pthreadfids[FCOUNT];

	//Initializing semaphores, mutexes etc here.
	sem_init(&capacity_semaphore, 0, RR_CAP);
	pthread_mutex_init(&semcheck, 0);
	pthread_mutex_init(&gendercheck, 0);
	srandom(time(NULL) % (unsigned int) RAND_MAX);

	pthread_attr_init(&attribs);
	
	for (i=0; i< MCOUNT; i++)		// creating male threads
	{
		tmids[i] = i;
		pthread_create(&(pthreadmids[i]), &attribs, thread_mmain, &(tmids[i]));
	}

	for (j=0; j< FCOUNT; j++)		// creating female threads
	{
		tfids[j] = j;
		pthread_create(&(pthreadfids[j]), &attribs, thread_fmain, &(tfids[j]));
	}
	
	for (i=0; i< MCOUNT; i++)		// joining male threads
		pthread_join(pthreadmids[i], NULL);
	
	for (j=0; j< FCOUNT; j++)		// joining female threads
		pthread_join(pthreadfids[j], NULL);

	return 0;
}

void *thread_mmain(void * arg)			// running the male threads
{
	int tid =  * ((int *) arg);
	threadMIDs[tid] = pthread_self();
    
	while(1==1)
	{
		do_other_stuff(0);
		man_enter();
		use_rr(0);
		man_leave();
	}
}

void *thread_fmain(void * arg)			// running female threads
{
	int tid =  * ((int *) arg);
	threadFIDs[tid] = pthread_self();

	while(1==1)
	{
		do_other_stuff(1);
		woman_enter();
		use_rr(1);
		woman_leave();
	}
}

void man_enter()				// Man entry code here
{
	int id = get_simple_tid(pthread_self(), 0);

	printf("Male Thread %d trying to get in the restroom\n", id);
	if (value==RR_CAP) // checking semaphore value is equal to the RR_CAP
	{
	     printf("\n Restroom Capacity = %d\n \n", value);
	     printf("\n ******** RESTROOM IS EMPTY NOW ******** \n");
         // locking the global variable to ensure single thread changes its value
	     pthread_mutex_lock(&gendercheck);
	     GENDER_CHECK=1;
         // unlocking the global variable to ensure single thread changes its value
	     pthread_mutex_unlock(&gendercheck);
	     pthread_mutex_lock(&semcheck);
         sem_wait(&capacity_semaphore);
	     sem_getvalue(&capacity_semaphore, &value);
	     printf("\nMale Thread %d got in!\n", id);
	     printf("\n Restroom Capacity = %d\n \n", value);
	     pthread_mutex_unlock(&semcheck);
	}
	else 
	{
	     if (GENDER_CHECK==1) // checking if restroom gender is male
	     {
              // locking the global variable to ensure single thread changes its value
	          pthread_mutex_lock(&semcheck);
		      sem_wait(&capacity_semaphore);
		      sem_getvalue(&capacity_semaphore, &value);
	          printf("Male Thread %d got in!\n", id);
		      printf("\n Restroom Capacity = %d\n \n", value);
              // unlocking the global variable to ensure single thread changes its value
	          pthread_mutex_unlock(&semcheck);
	     }
	     else 
	     {
	          while (GENDER_CHECK==2) 
		      { /* Do Nothing. Just Wait till GENDER_CHECK is 1. */ }
	          man_enter();	
	     }
	}
}

void man_leave()				// Man exit code here
{
	int id = get_simple_tid(pthread_self(), 0);

	printf("Male Thread %d leaving the restroom\n", id);
	sem_post(&capacity_semaphore);
	sem_getvalue(&capacity_semaphore, &value);
	printf("\n Restroom Capacity = %d\n \n", value);
	if (value==RR_CAP)
	 {
	  pthread_mutex_lock(&gendercheck);
	   GENDER_CHECK=0;
	  pthread_mutex_unlock(&gendercheck);
	 }
}

void woman_enter()				// Woman entry code here
{
	int id = get_simple_tid(pthread_self(), 1);

	printf("Female Thread %d trying to get in the restroom\n", id);
	if (value==RR_CAP) // checking semaphore value is equal to the RR_CAP
	{
	 printf("\n Restroom Capacity = %d\n \n", value);
	 printf("\n ******** RESTROOM IS EMPTY NOW ******** \n");
    // locking the global variable to ensure single thread changes its value
	 pthread_mutex_lock(&gendercheck);
	 GENDER_CHECK=2;
    // unlocking the global variable to ensure single thread changes its value
	 pthread_mutex_unlock(&gendercheck);
         pthread_mutex_lock(&semcheck);
		sem_wait(&capacity_semaphore);
		sem_getvalue(&capacity_semaphore, &value);
	  	printf("\nFemale Thread %d got in!\n", id);
		printf("\n Restroom Capacity = %d\n \n", value);
	 pthread_mutex_unlock(&semcheck);
	}
	else 
	{
	 if (GENDER_CHECK==2)
	 {
	  pthread_mutex_lock(&semcheck);
		sem_wait(&capacity_semaphore);
		sem_getvalue(&capacity_semaphore, &value);
	  	printf("Female Thread %d got in!\n", id);
		printf("\n Restroom Capacity = %d\n \n", value);
	  pthread_mutex_unlock(&semcheck);
	 }
	 else
	 {
	  while (GENDER_CHECK==1)
		{ /* Do Nothing. Just Wait till GENDER_CHECK is 2. */ }
	   woman_enter();
	 }
	}
}

void woman_leave()				// Woman exit code here
{
	int id = get_simple_tid(pthread_self(), 1);

	printf("Female Thread %d leaving the restroom\n", id);
	sem_post(&capacity_semaphore);
	sem_getvalue(&capacity_semaphore, &value);
	printf("\n Restroom Capacity = %d\n \n", value);
	if (value==RR_CAP)
	  {
	   pthread_mutex_lock(&gendercheck);
	   GENDER_CHECK=0;
	   pthread_mutex_unlock(&gendercheck);
	  }
}

void use_rr(int x)				// using the restroom in this function
{
	struct timespec req, rem;
	double usetime;
	usetime = RR_MAXSLEEP * (random() / (1.0*(double) ((unsigned long) RAND_MAX)));
	req.tv_sec = (int) floor(usetime);
	req.tv_nsec = (unsigned int) ( (usetime - ((int) floor(usetime))) * 1000000000);
	if (x==0)
	{
	   printf("Male Thread %d using restroom for %lf time\n", get_simple_tid(pthread_self(), 0), usetime);
	   nanosleep(&req,&rem);
	}
	else if (x==1)
	{
	   printf("Female Thread %d using restroom for %lf time\n", get_simple_tid(pthread_self(), 1), usetime);
	   nanosleep(&req,&rem);
	}
}

void do_other_stuff(int y)			// do other stuff in this function
{
	struct timespec req, rem;
	double worktime;
	worktime = OTHER_SLEEP * (random() / (1.0*(double) ((unsigned long) RAND_MAX)));
	req.tv_sec = (int) floor(worktime);
	req.tv_nsec = (unsigned int) ( (worktime - (int) floor(worktime)) * 1000000000);
	if (y==0)
	{
	printf("Male Thread %d working for %lf time\n", get_simple_tid(pthread_self(), 0), worktime);
	nanosleep(&req,&rem);
	}
	else if (y==1)
	{
	printf("Female Thread %d working for %lf time\n", get_simple_tid(pthread_self(), 1), worktime);
	nanosleep(&req,&rem);
	}
}

int get_simple_tid(pthread_t lid, int a)
{
	int i;
        if (a == 0)
        { 
	  for (i=0; i<MCOUNT; i++)
		if (pthread_equal(lid, threadMIDs[i]))
			return i;
	printf("Oops! did not find a tid for %lu\n", lid);
	_exit(-1);
	}
	else if (a == 1)
	{
	  for (i=0; i<FCOUNT; i++)
		if (pthread_equal(lid, threadFIDs[i]))
			return i;
	printf("Oops! did not find a tid for %lu\n", lid);
	_exit(-1);
	}
}