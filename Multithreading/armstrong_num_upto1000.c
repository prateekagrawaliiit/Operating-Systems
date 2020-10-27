/*
* @Author: prateek
* @Date:   2020-10-28 01:29:39
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-28 01:53:03
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>

void *run1(void *param); //calculate the cube of each digits of the number
void *run2(void *nums); //check if the original number and cubes are equal or not

int main(int argc, char* argv[])
{

    pthread_t tid;
    pthread_attr_t attr;
    if (argc < 2)
    {
        printf("Wrong Usage\n");
        return 0;
    }
    int n = atoi(argv[1]);
    for (int i = 1; i <= n; i++)
    {
        int *original = (int*)malloc(sizeof(int));
        original[0] = i;
        pthread_attr_init(&attr);
        pthread_create(&tid, NULL, run1, (void *)original);
        pthread_join(tid, NULL);
        free(original);

    }

    return 0;

}

void *run1(void *param)
{
    pthread_t tid;
    pthread_attr_t attr;
    int *num1 = param;
    int original = *num1;
    int result = 0;
    int remainder = 0;
    while (original != 0)
    {   
        remainder = (original % 10);
        result += remainder * remainder * remainder;
        original /= 10;
    }
    printf("%d %d\n",*num1,result);
    int *nums = (int*)malloc(2 * sizeof(int));
    nums[0] = *num1;
    nums[1] = result;
    pthread_attr_init(&attr);
    pthread_create(&tid, NULL, run2 , (void *)nums);
    pthread_join(tid, NULL);
    pthread_exit(0);
}
void *run2(void *nums)
{
    int *param=(int *)nums;
    if(param[0]==param[1])
    {
        printf("%d is an Armstrong number\n",param[0]);
    }
    pthread_exit(0);1
}
