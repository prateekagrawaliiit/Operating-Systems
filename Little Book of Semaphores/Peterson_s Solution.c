/*
* @Author: prateek
* @Date:   2020-11-28 23:03:28
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-28 23:24:45
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Size of the buffer
#define BufferSize 4

// Thread producer has a code 0
#define prod 0
#define cons 1
// Next free location in the buffer
int in = 0;
// First full location in the buffer
int out = 0;
int buffer[BufferSize];
int flag[2] = {0,0};
int turn =0;


void *runner_producer(void *param)
{
    int item;
    // Entry Section
    flag[prod] = 1;
    turn = cons;
    // Busy Wait
    while((flag[cons] ==1) && (turn==cons));

//      Critical Section
        item = rand()%10;
        buffer[in] = item;
        printf("The producer produced the item : %d\n",item);
        in = (in+1)%BufferSize;

//  Exit Section
    flag[prod] = 0;

}

void *runner_consumer(void *param)
{
    int item;
    // Entry Section
    flag[cons] = 1;
    turn = prod;
    // Busy Wait
    while((flag[prod]==1) && (turn==prod));

//      Critical Section
        item = buffer[out];
        printf("The consumer consumed the item : %d\n",item);
        out = (out+1)%BufferSize;

//  Exit Section
    flag[cons] = 0;

}

int main(int argc,char* argv[])
{

    srand(time(0));
    pthread_t pro[4],con[4];
    for(int i = 0; i < 4; i++) {
        pthread_create(&pro[i], NULL, (void *)runner_producer, NULL);
        pthread_create(&con[i], NULL, (void *)runner_consumer, NULL);
        
    }
    
    for(int i = 0; i < 4; i++) {
        pthread_join(pro[i], NULL);
        pthread_join(con[i], NULL);
    }



    return 1;

}