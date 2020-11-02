// CPP Program to perform binary search using pthreads 
#include <algorithm> 
#include <chrono> 
#include <iostream> 
#include<vector>
#include<bits/stdc++.h>
using namespace std; 
using namespace std::chrono;

// size of array 
#define MAX 16

// maximum number of threads 
#define MAX_THREAD 4 

// array to be searched 
int arr[] = { 1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220}; 

// key that needs to be searched 
int key = 64; 
bool found=false; 
int part; 

void* exponential(void* arg) 
{ 
	
	// Each thread checks 1/4 of the array for the key 
	int thread_part = part++; 
	int mid; 
	if (arr[0] == key)
        found=true;
    int i = 1;
    while (i < MAX && arr[i] <= key)
        i = i*2;
	int low = thread_part * (i/2);
	int high = (thread_part + 1) * min(i, MAX-1);
	while (low < high && !found) 
	{ 
		mid = (high - low) / 2 + low; 

		if (arr[mid] == key) { 
			found = true; 
			break; 
		} 

		else if (arr[mid] > key) 
			high = mid - 1; 
		else
			low = mid + 1; 
	} 
pthread_exit(0);
} 



int main() 
{ 
	pthread_t threads[MAX_THREAD]; 

	auto start = high_resolution_clock::now();
	for (int i = 0; i < MAX_THREAD; i++) 
		pthread_create(&threads[i], NULL, exponential, (void*)NULL); 
	for (int i = 0; i < MAX_THREAD; i++) 
		pthread_join(threads[i], NULL); 
	if (found) 
		cout << key << " found in array" << endl;
	else
		cout << key << " not found in array" << endl; 
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by exponential: "
         << duration.count() << " microseconds" << endl;

	return 0; 
} 
