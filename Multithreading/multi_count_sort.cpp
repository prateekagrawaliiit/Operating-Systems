/*
* @Author: prateek
* @Date:   2020-10-31 16:28:08
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-31 16:54:21
*/

#include <iostream>
#include <limits.h>
#include<pthread.h>
#include<stdlib.h>

using namespace std;

// size of array
#define MAX 16

// maximum number of threads
#define MAX_THREAD 4

// array to be searched
int a[] = { 16, 16, 14, 12, 12, 13, 11, 19, 18, 3, 6, 2, 4, 3, 2, 1};
int occ[100000] = {0};

void* count_sort(void* arg)
{
    int *params = (int*)arg;


    // Each thread checks 1/4 of the array for the key
    int thread_part = params[0];
    printf("Thread Part : %d\n", thread_part);
    int low = thread_part * (MAX / 4);
    int high = (thread_part + 1) * (MAX / 4);


    // search for the key until low < high
    // or key is found in any portion of array
    for (int i = low; i < high; i++)
        occ[a[i]] += 1;

    return NULL;

}


// Driver Code
int main()
{
    pthread_t threads[MAX_THREAD];
    for (int i = 0; i < MAX_THREAD; i++)
    {   int *part = (int*)malloc(sizeof(int));

        part[0] = i;
        pthread_create(&threads[i], NULL, count_sort, part);
    }

    for (int i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);


    for (int i = 0; i < 100000; i++)
        while (occ[i] != 0)
        {
            printf("%d ", i);
            occ[i] -= 1;
        }

    return 0;
}