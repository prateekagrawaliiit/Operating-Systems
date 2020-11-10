/*
* @Author: prateek
* @Date:   2020-11-10 19:46:13
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-10 19:48:09
*/
#include <stdio.h>
#include <pthread.h>

#define N 5


void *runner(void *params);

int main(int argc, char* argv[])
{
    int i, policy;
    pthread_t tid[N];
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    if (pthread_attr_getschedpolicy(&attr, &policy) != 0)
    {
        fprintf(stderr, "Unable to get policy\n");
    }
    else
    {
        if (policy == SCHED_OTHER)
            printf("SCHED_OTHER\n");
        else if (policy == SCHED_RR)
            printf("SCHED_RR\n");
        else if (policy == SCHED_FIFO)
            printf("SCHED_FIFO\n");
        else
            fprintf(stderr, "INVALID POLICY VALUE\n");

    }

    if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
    {
        fprintf(stderr, "Unable to set policy\n");
    }

    for (i = 0; i < N; i++)
    {
        pthread_create(&tid[i], &attr, runner, NULL);
    }

    for (i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }



}

void *runner(void *params)
{

    for (int j = 0; j < 10; j++)
    {
        printf("%d", j);
        sleep(2);

    }
    printf("\n");
    pthread_exit(0);
}