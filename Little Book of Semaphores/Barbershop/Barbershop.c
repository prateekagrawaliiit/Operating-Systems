#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>

//Global Varibles
struct customerQueue *Head;
int customerCount;
pthread_mutex_t barberMutex;
pthread_mutex_t customerMutex;

//Struct
struct customerQueue {
	//int Number;
	struct customerQueue *Next;
};

//Functions
void *Barber();
void *Customer( void *arg );
void addCustomer();
void removeCustomer();
void cut_hair();
void get_hair_cut( int customerNumber );

int main(int argc, char *argv[]){

	//Create Processes
	pthread_t customerThread[6], barberThread;
	customerCount = 0;
	Head = NULL;

	//Init Mutex
	pthread_mutex_init( &barberMutex, NULL );
    	pthread_mutex_init( &customerMutex, NULL );

	//Create thread
	pthread_create( &barberThread, NULL, Barber, NULL );

	int i, id[6];
	for( i = 0; i < 6; i++){

		//Gives id numbers
		id[i]=i;

		//Creating thread
		pthread_create( &customerThread[i], NULL, Customer, (void*)( &id[i] ) );

	}

	while(1){

	}

	return 0;
}

void *Barber(){

	while(1){

		while( customerCount == 0 ){
			printf( "No Customers baber is sleeping\n" );            			
			sleep( 2 );
		}

		//Barber gives a hair cut
		cut_hair();
	
	}
}

void *Customer( void *arg ){
	
	int customerNumber = *(int *)arg;

	while( 1 ){

		//Check if overflow of Customers
		if( customerCount >= 3 ){
			printf( "All chairs are filled, Customer leaves.\n" );
			return 0;
		}

		//Lock
		pthread_mutex_lock( &customerMutex );

		//Add Customer
		addCustomer();

		//Unlock
		pthread_mutex_unlock( &customerMutex );


		//Lock
		pthread_mutex_lock( &barberMutex );

		//Get the haircut
		get_hair_cut( customerNumber );

		//Remove
		removeCustomer();

		//Unlock
		pthread_mutex_unlock( &barberMutex );

		sleep( 10 );

	}
}

void addCustomer(){

	int numberCustomer = 0;
	struct customerQueue *Queue;
	Queue = ( struct customerQueue * )malloc( sizeof( struct customerQueue ) );
	Queue->Next = NULL;

	if( Head == NULL ){
		Head = Queue;

	} else {

		struct customerQueue *addLink = Head;
		
		while( addLink->Next != NULL ){
			addLink = addLink->Next;
		}

		addLink->Next = Queue;

	}

	customerCount++;

}

void removeCustomer(){

	struct customerQueue *Queue = Head;
	Head = Head->Next;
	free( Queue );
	
	customerCount--;

}

void cut_hair(){
	printf( "Barber starts cutting hair\n" );
	fflush( stdout );

	sleep( 4 );

	printf( "Barber finishes haircut\n" );
	fflush( stdout );
	
}

void get_hair_cut( int customerNumber ){
	printf( "Customer %d starts getting an haircut\n", customerNumber );
	fflush( stdout );

	sleep( 4 );

	printf( "Customer %d got their haircut\n", customerNumber );
	fflush( stdout );

}
