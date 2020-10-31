/*
* @Author: prateek
* @Date:   2020-10-31 13:29:08
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-31 13:41:25
*/

#include <iostream>
#include <time.h>

using namespace std;

// size of array
#define MAkey 16

// makeyimum number of threads
#define MAkey_THREAD 4

// array to be searched
int a[] = { 1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220 };

// key that needs to be searched
int key = 110;
bool found = false;
int part = 0;

void* binary_search(void* arg)
{

    // Each thread checks 1/4 of the array for the key
    int thread_part = part++;
    int mid;

    int low = thread_part * (MAkey / 4);
    int high = (thread_part + 1) * (MAkey / 4);

    // search for the key until low < high
    // or key is found in any portion of array
    while (low < high && !found)  {

        // normal iterative binary search algorithm
        mid = (high - low) / 2 + low;

        if (a[mid] == key)  {
            found = true;
            break;
        }

        else if (a[mid] > key)
            high = mid - 1;
        else
            low = mid + 1;
    }


    return NULL;

}


void* interpolation_search(void* arg)
{

    // Each thread checks 1/4 of the array for the key
    int thread_part = part++;
    int mid;

    int lo = thread_part * (MAkey / 4);
    int hi = (thread_part + 1) * (MAkey / 4);

    // Since array is sorted, an element present
    // in array must be in range defined by corner
    while (lo <= hi && key >= a[lo] && key <= a[hi])
    {
        if (lo == hi) {
            if (a[lo] == key)
                printf("%d found in the array", key);
        }
        // Probing the position with keeping
        // uniform distribution in mind.
        int pos = lo + (((double)(hi - lo) /
                         (a[hi] - a[lo])) * (key - a[lo]));

        // Condition of target found
        if (a[pos] == key)
            printf("%d found in the array", key);

        // If key is larger, key is in upper part
        if (a[pos] < key)
            lo = pos + 1;

        // If key is smaller, key is in the lower part
        else
            hi = pos - 1;
    }
    return NULL;

}




// Driver Code
int main()
{

    clock_t t1, t2;


    pthread_t bthreads[MAkey_THREAD];

    t1 = clock();

    for (int i = 0; i < MAkey_THREAD; i++)
        pthread_create(&bthreads[i], NULL, binary_search, (void*)NULL);

    for (int i = 0; i < MAkey_THREAD; i++)
        pthread_join(bthreads[i], NULL);

    // key found in array
    if (found)
        cout << key << " found in array" << endl;

    // key not found in array
    else
        cout << key << " not found in array" << endl;
    t2 = clock();
    printf("Run time of binary_search: %f\n \n \n", (t2 - t1) / (double)CLOCKS_PER_SEC);


    part = 0;
    found = false;


    pthread_t ithreads[MAkey_THREAD];

    t1 = clock();


    for (int i = 0; i < MAkey_THREAD; i++)
        pthread_create(&ithreads[i], NULL, interpolation_search, (void*)NULL);

    for (int i = 0; i < MAkey_THREAD; i++)
        pthread_join(ithreads[i], NULL);

    // key found in array
    if (found)
        cout << key << " found in array" << endl;

    // key not found in array
    else
        cout << key << " not found in array" << endl;

    t2 = clock();
    printf("Run time of interpolation_search: %f\n", (t2 - t1) / (double)CLOCKS_PER_SEC);



    return 0;
}