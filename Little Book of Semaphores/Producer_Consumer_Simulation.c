/*
* @Author: prateek
* @Date:   2020-11-28 19:32:59
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-28 20:02:09
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
// Next free location in the buffer
int in = 0;
// First full location in the buffer
int out = 0;
int buffer[BufferSize];

void *runner_producer(void *param)
{
    int item;
    item = rand() % 10;
    buffer[in] = item;
    printf("The producer has produced the item : %d \n", item);
    item = (item + 1) % BufferSize;
}
void *runner_consumer(void *param)
{
    int item = buffer[out];
    printf("The consumer has consumed the item : %d \n", item);
    out = (out - 1) % BufferSize;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    pthread_t pro_id[4], con_id[4];
    for (int i = 0; i < 4; i++)
    {
        pthread_create(&pro_id[i], NULL, (void *)runner_producer, NULL);
        pthread_create(&con_id[i], NULL, (void *)runner_consumer, NULL);
    }
    for (int i = 0; i < 4; i++)
    {
        pthread_join(pro_id[i], NULL);
        pthread_join(con_id[i], NULL);
    }
    return 1;
}