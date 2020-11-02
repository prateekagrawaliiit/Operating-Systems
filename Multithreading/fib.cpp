// CPP Program to perform binary search using pthreads 
#include <iostream> 
#include <pthread.h>
using namespace std; 

// size of array 
#define MAX 16 

// maximum number of threads 
#define MAX_THREAD 4 

// Utility function to find minimum of two elements 
int min(int x, int y) { return (x<=y)? x : y; } 

// array to be searched 
int arr1[] = { 1, 5, 7, 10, 12, 14, 15, 18, 20, 22, 25, 27, 30, 64, 110, 220}; 

// key that needs to be searched 
int x = 110; 
bool found = false; 
int part = 0; 

void* binary_search(void* arg) 
{ 

	// Each thread checks 1/4 of the array for the key 

	int thread_part = part++; 

	int low = thread_part * (MAX / 4); 
	int high = (thread_part + 1) * (MAX / 4); 


	int arr[high] = {0};

	for (int i = low; i < high; i++)
	{
		arr[i-low] = arr1[i];
	}

	int n = 4;

	if (!found)
	{

	/* Initialize fibonacci numbers */
	int fibMMm2 = 0; // (m-2)'th Fibonacci No. 
	int fibMMm1 = 1; // (m-1)'th Fibonacci No. 
	int fibM = fibMMm2 + fibMMm1; // m'th Fibonacci 

	/* fibM is going to store the smallest Fibonacci 
	Number greater than or equal to n */
	while (fibM < high) 
	{ 
		fibMMm2 = fibMMm1; 
		fibMMm1 = fibM; 
		fibM = fibMMm2 + fibMMm1; 
	} 

	// Marks the eliminated range from front 
	int offset = -1; 

	/* while there are elements to be inspected. Note that 
	we compare arr[fibMm2] with x. When fibM becomes 1, 
	fibMm2 becomes 0 */
	while (fibM > 1) 
	{ 
		// Check if fibMm2 is a valid location 
		int i = min(offset+fibMMm2, n-1); 

		/* If x is greater than the value at index fibMm2, 
		cut the subarray array from offset to i */
		if (arr[i] < x) 
		{ 
			fibM = fibMMm1; 
			fibMMm1 = fibMMm2; 
			fibMMm2 = fibM - fibMMm1; 
			offset = i; 
		} 

		/* If x is greater than the value at index fibMm2, 
		cut the subarray after i+1 */
		else if (arr[i] > x) 
		{ 
			fibM = fibMMm2; 
			fibMMm1 = fibMMm1 - fibMMm2; 
			fibMMm2 = fibM - fibMMm1; 
		} 

		/* element found. return index */
		else 
		{
			found = true;
			break;
		}
	} 

	/* comparing the last element with x */
	if(fibMMm1 && arr[offset+1]==x)
	{
		found = true;
	}
}
pthread_exit(0);

} 



// Driver Code 
int main() 
{ 
	
	pthread_t threads[MAX_THREAD];



	for (int i = 0; i < MAX_THREAD; i++) 
		pthread_create(&threads[i], NULL, binary_search, (void*)NULL); 

	for (int i = 0; i < MAX_THREAD; i++) 
		pthread_join(threads[i], NULL); 

	// key found in array 
	if (found) 
		cout << x << " found in array" << endl; 

	// key not found in array 
	else
		cout << x << " not found in array" << endl; 

	return 0; 
} 
