//nxn Latin Square generation using thread-semaphore setup.
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
int s1=0,k=0,temp=0;int n;
int signal(int s)
{
	s++;
	return s;
}
void *gen1(void *params)
{

	temp = k; 
	while (temp <= n) 
	{ 
		printf("%d ", temp); 
		temp++; 
	}
	s1=signal(s1);
	pthread_exit(0);
}
void *gen2(void *params)
{
	while(s1==0)
	{
		if(s1>0)
			break;
	}
	for (int j=1; j<k; j++) 
		printf("%d ", j); 
	k--; 
	pthread_exit(0);
}
void printLatin(int n) 
{ 	
	k = n+1; 
	// Loop to print rows 
	for (int i=1; i<=n; i++) 
	{ 
		pthread_t tid[2];
		pthread_create(&tid[0],NULL,gen1,(void *)NULL);
		pthread_create(&tid[1],NULL,gen2,(void *)NULL);
		int i=0;
		while(i<2)
		{
			pthread_join(tid[i],NULL);i++;
		}
		printf("\n"); 
	} 
} 

// Driver program to test above function 
int main(void) 
{ 
	n = 9; 
	printLatin(n); 

	return 0; 
} 
