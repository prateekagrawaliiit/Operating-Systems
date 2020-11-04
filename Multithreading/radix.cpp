/*
* @Author: prateek
* @Date:   2020-10-31 13:50:43
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-31 13:51:48
*/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int arrradix[100];
int array[100];

struct data
{
	int arr[100];
	int n;
};

struct count_sort_data
{
	struct data *d;
	int exp;
};

int getMax(int arr[], int n) 
{ 
    int mx = arr[0]; 
    for (int i = 1; i < n; i++) 
        if (arr[i] > mx) 
            mx = arr[i]; 
    return mx; 
} 

void * countSort(struct count_sort_data * param) 
{ 
	struct count_sort_data cdd = *param;
	struct count_sort_data *cd = &cdd;

    int output[(cd->d)->n]; // output array 
    int i, count[10] = {0}; 

    for (i = 0; i < (cd->d)->n; i++) 
        count[ ((cd->d)->arr[i]/cd->exp)%10 ]++; 

    for (i = 1; i < 10; i++) 
        count[i] += count[i - 1]; 
  
    // Build the output array 
    for (i = (cd->d)->n - 1; i >= 0; i--) 
    { 
        output[count[ ((cd->d)->arr[i]/cd->exp)%10 ] - 1] = (cd->d)->arr[i]; 
        count[ ((cd->d)->arr[i]/cd->exp)%10 ]--; 
    } 

    for (i = 0; i < (cd->d)->n; i++) 
        (cd->d)->arr[i] = output[i]; 

    pthread_exit(0);
} 

void * RadixSort(void * param) 
{  
	struct data *d = param;

    int m = getMax(d->arr, d->n); 

    

    int noofthreads = 0;
    for (int exp = 1; m/exp > 0; exp *= 10)
    {
    	noofthreads++;
    }

    struct count_sort_data cd[noofthreads];

    pthread_t tid[noofthreads];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	int i=0;
    for (int exp = 1; m/exp > 0; exp *= 10)
    {
    	cd[i].d = d;
    	cd[i].exp = exp;
    	pthread_create(&tid[i], &attr, countSort, &cd[i]);
    	pthread_join(tid[i], NULL);
    	i++;
    }

  //   for(int j=0;j<noofthreads;j++)
  //   {
		// pthread_join(tid[j], NULL);    	
  //   }

    pthread_exit(0);
} 
 
// void print(int arr[], int n)
// { 
//     for (int i = 0; i < n; i++) 
//         cout << arr[i] << " "; 
// }

void * BubbleSort(void * param)
{
	struct data *d = param;

	for(int i=1;i<d->n;i++){
		int tcheck=0;
		for(int j=0;j<d->n-i;j++){
			if(d->arr[j]>d->arr[j+1]){
				int temp=d->arr[j];
				d->arr[j]=d->arr[j+1];
				d->arr[j+1]=temp;
				tcheck=1;
			}
		}
		if(tcheck==0)break;
	}

	pthread_exit(0);
}

void * InsertionSort(void * param)
{
	struct data *d = param;

	for(int i=1;i<d->n;i++){
		for(int j=i;j>0;j--){
			if(d->arr[j-1]>d->arr[j]){
				int temp=d->arr[j];
				d->arr[j]=d->arr[j-1];
				d->arr[j-1]=temp;
			}
			else break;
		}
	}

	pthread_exit(0);
}

int main(int argc, char *argv[])
{


	if(argc < 2) printf("Wrong number of parameters.\n");
	else 
	{
		int n = argc - 1;

		for(int i=0;i<n;i++)
		{
			arrradix[i] = atoi(argv[1 + i]);
			array[i] = atoi(argv[1 + i]);
		}

		struct data d[3];
		for(int i=0;i<n;i++)
		{
			d[0].arr[i] = arrradix[i];
			d[1].arr[i] = arrradix[i];
			d[2].arr[i] = arrradix[i];
		}
		d[0].n = n;
		d[1].n = n;
		d[2].n = n;

		pthread_t tid[2];
		pthread_attr_t attr;
		pthread_attr_init(&attr);

		pthread_create(&tid[0], &attr, RadixSort, &d[0]);
		pthread_create(&tid[1], &attr, BubbleSort, &d[1]);
		pthread_create(&tid[1], &attr, InsertionSort, &d[2]);

		pthread_join(tid[0], NULL);
		pthread_join(tid[1], NULL);
		pthread_join(tid[2], NULL);

		printf("Radix Sorted: ");

		for(int i=0;i<n;i++) printf("%d ", d[0].arr[i]);

		printf("\n");

		printf("Bubble Sorted: ");

		for(int i=0;i<n;i++) printf("%d ", d[1].arr[i]);

		printf("\n");

		printf("Insertion Sorted: ");

		for(int i=0;i<n;i++) printf("%d ", d[2].arr[i]);

		printf("\n");
	}
	

	
}