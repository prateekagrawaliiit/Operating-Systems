/*
* @Author: prateek
* @Date:   2020-10-28 18:37:29
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-28 18:41:28
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>

void *runner(void *nums);

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int n = atoi(argv[1]);
    for (int i = 1; i <= n; i++)
    {
        int *num = (int*)malloc(sizeof(int));
        num[0] = i;
        pthread_attr_init(&attr);
        pthread_create(&tid, NULL, runner, num);
        pthread_join(tid, NULL);
        free(num);

    }

    return 0;
}

void *runner(void *num)
{
    int *param = (int *)num;
    int x = param[0];
    int count = 0;
    for (int i = 1; i <= x; i++)
    {
        if (x % i == 0)
            count++;
    }
    if(count==2)
        printf("%d is a Prime Number\n",x);

    pthread_exit(0);


}