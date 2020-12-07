#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include "queue.h"

/*
 * Variables to indicate the number of instances per resource
*/
int num_clients = 0;
int num_chairs = 3;
int num_barbers = 3;
int num_seats_sofa = 4;
int num_max_costumers = 20;


/*
 * Threads to run costumers' and barbers' actions
*/
pthread_t *thr_costumers;
pthread_t *thr_barbers;


/*
 * Queue for the costumers in the waiting room and for the sofa
*/
struct Queue *q_waiting_room, *q_sofa; 

/*
 * Semaphores for each one of the resources and one master key (mutex)
*/
sem_t queue_wr;
sem_t queue_s;
sem_t queue_c;
sem_t chair;
sem_t barber;
sem_t costumer;
sem_t payment;
sem_t cash_register;

sem_t waiting_room_empty, sofa_empty;

char* barbers [13] = {"Jerry Smith", "Nando DK", "Jhowzinho", "Kadinho", "Livinho", "Kevinho", "G15", "Gustta", "Lucas Lucco", "Yuri Martins", "Lan", "WM", "Don Juan"};

/*
 * Display a message to show that the barbershop is full
*/
void shop_is_full(int clientID) {
	printf("C%d: X The shop is full. No more costumers are allowed to enter. Client is leaving the store.\n", clientID);
}

/*
 * Display a message that the barber started to cut the hair
*/
void cutting_hair(int barberID) {
	printf("Barber %s is going to cut the hair.\n", barbers[barberID]);
	sleep(rand()%6+4);
}

/*
 * Display a message that the barber will receive the payment
*/
void receiving_payment(int barberID) {
	printf("Barber %s is receiving the payment.\n", barbers[barberID]);
	sleep(rand()%4);
}

/*
 * Display a message that the costumer is going to the waiting room
*/
void going_to_waiting_room(int clientID) {
	if(isFull(q_sofa)){
		printf("C%d: 0 The client is entering the waiting room.\n", clientID);
		sleep(rand()%2);
	}
	sleep(1);
}

/*
 * Display a message that the costumer is going to the seat on the sofa
*/
void going_to_the_sofa(int clientID) {
	printf("C%d: 1 The client is seating on the sofa.\n", clientID);
	sleep(rand()%2);
}

/*
 * Display a message that the costumer is going to receive a haircut
*/
void going_to_the_chair(int clientID) {
	printf("C%d: 2 Ah, ah, aah. Now you gonna seat!.\n", clientID);
	sleep(rand()%2);
}

/*
 * Display a message that the costumer finished to have the haircut
*/
void got_hair_cut(int clientID) {
	printf("C%d: 3 The client is now avonts and is going to pay the bill.\n", clientID);
	sleep(rand()%3);
}

/*
 * Display a message that the costumer is ready to leave the shop
*/
void client_is_leaving(int clientID) {
	printf("C%d: 4 The client is tranks and is leaving the shop.\n", clientID);
	sleep(rand()%3);
}

/*
 * Executes the costumer's actions.
 * Mutex is used when the number of costumers needs to be changed.
 * Other semaphores are used to access other non critical resources, such as:
 * for the sofa, chairs, barbers and costumers.
 *
 * void *v: is the costumer ID
*/
void* f_costumer (void *v) {
	int clientID = *(int *) v;
	if (num_clients >= num_max_costumers) {
		shop_is_full(clientID);
	}
	++num_clients;
	sem_wait(&waiting_room_empty);
	sem_wait(&queue_wr);
	enqueue(q_waiting_room,clientID);
	going_to_waiting_room(clientID);
	sem_post(&queue_wr);
	sem_wait(&sofa_empty);
	sem_wait(&queue_s);
	clientID = dequeue(q_waiting_room);
	sem_post(&waiting_room_empty);
	enqueue(q_sofa,clientID);
	going_to_the_sofa(clientID);
	sem_post(&queue_s);
    sem_wait(&chair);
    sem_wait(&queue_c);
	clientID = dequeue(q_sofa);
	going_to_the_chair(clientID);
	sem_post(&queue_c);
	sem_post(&sofa_empty);
	sem_post(&costumer);
	sem_wait(&barber);
	got_hair_cut(clientID);
	sem_post(&payment);
	sem_wait(&cash_register);
	--num_clients;
	client_is_leaving(clientID);
}

/*
 * Executes the barber's actions, while there are still costumers.
 * Actions include decrease the number of costumers that need to receive a haircut,
 * increase the number of barbers working, increase the number of chairs occupied,
 * receive the payment and use the cash register.
 *
 * void *v: is the barber ID
*/
void* f_barber (void *v) {
	int barberID = *(int *) v;
	
	while(1) {
		sem_wait(&costumer);
		sem_post(&barber);
		cutting_hair(barberID);
		sem_post(&chair);
		sem_wait(&payment);
		receiving_payment(barberID);
		sem_post(&cash_register);
	}
}

/*
 * Checks if the number of command line arguments is correct. The first argument is always the program name
 */
int check_arguments(int argc, char* argv[]) {
	if (argc != 5) {
		printf("Usage: %s <number of chairs> <number of barbers> <number of seats> <maximum nunmber of costumers>\nUsing the default arguments..\n\n", argv[0]);

		//exit(0);
		return 0;
	}
	return 1;
}

/*
 * Runs the barbershop. Checks the arguments, initialize the semaphores, creates the costumers and barbers.
*/
int main(int argc, char* argv[]) {
	if(check_arguments(argc, argv)) {

		num_chairs = atoi(argv[1]);
		num_barbers = atoi(argv[2]);
		num_seats_sofa = atoi(argv[3]);
		num_max_costumers = atoi(argv[4]);
	}

    srand( (unsigned)time(NULL) );
	thr_costumers = (pthread_t *)malloc(sizeof(pthread_t) * num_max_costumers);
	thr_barbers = (pthread_t *)malloc(sizeof(pthread_t) * num_barbers);
	
	q_waiting_room = createQueue(num_max_costumers-num_seats_sofa);
	q_sofa = createQueue(num_seats_sofa);

	int client[num_max_costumers];

	sem_init(&chair, 0, num_chairs);
	sem_init(&barber, 0, 0);
	sem_init(&costumer, 0, 0);
	sem_init(&payment, 0, 0);
	sem_init(&cash_register, 0, 0);
    sem_init(&queue_wr,0,1);
    sem_init(&queue_s,0,1);
    sem_init(&queue_c,0,1);
	sem_init(&waiting_room_empty, 0, num_max_costumers - num_seats_sofa);
	sem_init(&sofa_empty, 0, num_seats_sofa);

	int i = 0;

    int al = rand() % 10;
    int shuffle[num_barbers];

	for (i = 0; i < num_max_costumers; i++) {
		client[i] = i;

		pthread_create(&thr_costumers[i], 0, f_costumer, &client[i]);
	}

	sleep(20000);

	free(thr_costumers);
	free(thr_barbers);
	
	return 0;
}
