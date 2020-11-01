/*
* @Author: prateek
* @Date:   2020-11-01 10:17:38
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-01 12:23:25
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <pthread.h>
int n = 0;
int sub_array_size = 0;
int ar[1000000];
int arr1[1000000];
int arr2[1000000];
int part = 0;
void *sort_sub_runner(void *params);
void *final_runner(void *params);
int main(int argc, char const *argv[])
{
    // Get the size of the input
    n = argc - 1;
    // Create an array of size n
    int ar[n];
    // Initialize the array with the values
    for (int i = 0; i < n; ++i)
    {
        ar[i] = atoi(argv[i + 1]);
    }
    // declare the size of the first sub-array
    sub_array_size = n / 2;
    //  THREAD 1
    // Declare the first thread to sort the first half of the array
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // pass the first half of the array as input parameter to the runner function.
    // Initialize the parameter array
    int *sub1 = (int*)malloc((sub_array_size + 1) * sizeof(int));
    sub1[0] = sub_array_size;
    for (int i = 0; i < sub_array_size; i++)
        sub1[i + 1] = ar[i];
    // Create the first theard to sort the first half of the array.
    pthread_create(&tid, &attr, sort_sub_runner, sub1);
    // THREAD 2
    // Declase the second thread
    pthread_t tid2;
    // Initialize the parameters of the second sub array thread with the second half of the array.
    int *sub2 = (int*)malloc(((n - sub_array_size) + 1) * sizeof(int));
    sub2[0] = n - sub_array_size;
    for (int i = sub_array_size; i < n; i++)
    {
        sub2[(i + 1 - sub_array_size)] = ar[i];
    }

    // Create the second thread to sort and handle the second half of the array.
    pthread_create(&tid2, &attr, sort_sub_runner, sub2);
    // Ensure that the parent waits for both these threads to complete so that it creates and executes it.
    pthread_join(tid, NULL);
    pthread_join(tid2, NULL);
    // Create final merger thread to merge these sorted sub arrays to create the final sorted array.
    pthread_t final;
    pthread_create(&final, &attr, final_runner, NULL);
    pthread_join(final, NULL);
}
void *sort_sub_runner(void *params)
{

    // Get the parameters into a new array
    int *nums = (int*)params;
    // The first index corresponds to the size of the sub array get that
    int n1 = nums[0];

    // Increase the value of part which is used to refer to the thraed.
    part++;

    // If first thread is begin executed work on the first sub array
    if (part == 1)
    {
        // Bubble sort to sort the first sub array and store it in the global sub array arr1
        for (int i = 0; i < n1; i++)
            arr1[i] = nums[i + 1];
        for (int i = 1; i < n1; i++) {
            int tcheck = 0;
            for (int j = 0; j < n1 - i; j++) {
                if (arr1[j] > arr1[j + 1]) {
                    int temp = arr1[j];
                    arr1[j] = arr1[j + 1];
                    arr1[j + 1] = temp;
                    tcheck = 1;
                }
            }
            if (tcheck == 0)break;
        }
        pthread_exit(0);
    }

    // If second thread is begin executed work on the second sub array

    else if (part == 2)
    {
        // Bubble sort to sort the second sub array and store it in the global sub array arr2

        for (int i = 0; i < n1; i++)
            arr2[i] = nums[i + 1];
        for (int i = 1; i < n1; i++) {
            int tcheck = 0;
            for (int j = 0; j < n1 - i; j++) {
                if (arr2[j] > arr2[j + 1]) {
                    int temp = arr2[j];
                    arr2[j] = arr2[j + 1];
                    arr2[j + 1] = temp;
                    tcheck = 1;
                }
            }
            if (tcheck == 0)break;
        }
        pthread_exit(0);
    }
    return NULL;
}
void *final_runner(void *params)
{


    // get the sizes of each of the individual sub arrays
    int n1 = sub_array_size;
    int n2 = n - sub_array_size;

    int ar3[n];
    int ar4[n1], ar5[n2];

    // copy the values from the global sub arrays to new arrays to prevent data mishandling

    for (int i = 0; i < n1; ++i)
    {
        ar4[i] = arr1[i];
    }
    for (int i = 0; i < n2; ++i)
    {
        ar5[i] = arr2[i];
    }


    // Merge the sorted sub arrays into a new array in ascending order. 
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2)
    {
        if (ar4[i] < ar5[j])
            ar3[k++] = ar4[i++];
        else
            ar3[k++] = ar5[j++];
    }
    while (i < n1)
        ar3[k++] = ar4[i++];
    while (j < n2)
        ar3[k++] = ar5[j++];


    // ar3 is the final array that is sorted and lets print it out 
    for (int l = 0; l < n; ++l)
    {
        printf("%d\n", ar3[l]);
    }

    return NULL;
}