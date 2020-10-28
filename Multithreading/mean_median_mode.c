/*
* @Author: prateek
* @Date:   2020-10-28 18:43:38
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-29 00:10:03
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>

void *runner_mean(void *nums);
void *runner_median(void *nums);
void *runner_mode(void *nums);


int cmpfunc(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int main(int argc,char* argv[])
{
    int n = argc-1;
    int *nums = (int *)malloc((n+1)*sizeof(int));
    nums[0] = n;
    for(int i=1;i<=n;i++)
        nums[i] = atoi(argv[i]);

    pthread_t tid_mean;
    pthread_create(&tid_mean,NULL,runner_mean,nums);
    pthread_join(tid_mean,NULL);

    pthread_t tid_median;
    pthread_create(&tid_median,NULL,runner_median,nums);
    pthread_join(tid_median,NULL);

    pthread_t tid_mode;
    pthread_create(&tid_mode,NULL,runner_mode,nums);
    pthread_join(tid_mode,NULL);

}

void *runner_mean(void *nums)
{
    int *params = (int *)nums;
    int n= params[0];
    float sum =0;
    for(int i=1;i<=n;i++)
        sum+=params[i];
    printf("%0.2f is the mean of the given array\n", (sum/n));

    pthread_exit(NULL);


}

void *runner_median(void *nums)
{
    int *params = (int *)nums;
    int size = params[0];
    int *numbers =(int *)malloc(size*sizeof(int));
    for(int i=0;i<size;i++)
        {
            numbers[i] = params[i+1];
        }
    qsort(numbers,size,sizeof(int),cmpfunc);
    
    if(size%2==0)
    printf("%0.2f is the median of the given array\n", (float)(numbers[(int)size/2] + numbers[(int)size/2 +1])/2);
    else
    printf("%0.2f is the median of the given array\n", (float)(numbers[(int)size/2]));

    pthread_exit(NULL);

    
}

void *runner_mode(void *nums)
{
    int *params = (int *)nums;
    int size = params[0];
    int *numbers =(int *)malloc(size*sizeof(int));
    for(int i=0;i<size;i++)
        {
            numbers[i] = params[i+1];
        }
    qsort(numbers,size,sizeof(int),cmpfunc);
    
    int count1 = 1, count2 = 1;
    int i = 0, index = 1;
    int mo = numbers[0];

    for (i = 0; i < size; i++)
    {
        if (numbers[i] == numbers[i + 1])
        {
            while (numbers[i] == numbers[i + 1])
            {
                count1++;
                i++;
            }
            if (count1 > count2)
            {
                count2 = count1;
                mo = numbers[i];
                count1 = 1;
            }
        }
    }

    printf("%d is the mode of the given array\n",mo);

    pthread_exit(NULL);


    
}
