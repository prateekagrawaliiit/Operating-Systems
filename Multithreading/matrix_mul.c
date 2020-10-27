/*
* @Author: prateek
* @Date:   2020-10-28 00:56:42
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-28 01:19:56
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>


#define NUM_THREADS 10
#define M 3
#define K 2
#define N 3

int A[M][K] = {{2, 2}, {2, 2}, {3, 6}};
int B[K][N] = {{8, 7, 6}, {5, 4, 3}};
int C[M][N];

struct v
{
    int i;//row index
    int j; //column index
};
void *runner(void *params); // The thread


int main()
{
    int i, j, count = 0;
    for (i = 0; i < M; i++)
    {
        for (j = 0; j < N; j++)
        {
            struct v *data = (struct v*)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tid, &attr, runner, data);
            pthread_join(tid, NULL);
        }
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }




    return 0;
}


void *runner(void *param)
{
    struct v *data = param;
    int n=0,sum=0;
    for(n=0;n<K;n++)
    {
        sum+= A[data->i][n] * B[n][data->j];
    }
    C[data->i][data->j] = sum;
    pthread_exit(0);
}