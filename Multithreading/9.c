#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
long long int fib=1,fib1=1,fib2=1;
void *gen1(void *param);
void *gen2(void *param);
int main(int argc, char const *argv[])
{
	pthread_t tid1,tid2;
	pthread_attr_t attr1,attr2;
	printf("0\n");
	printf("1\n");
	int n=atoi(argv[1]);
	for(int i=2;i<=n;i++)
	{
    int *nums1 = (int*)malloc(sizeof(int));
    int *nums2 = (int*)malloc(sizeof(int));
    nums1[0]=i-1;nums2[0]=i-2;
		pthread_attr_init(&attr1);
		pthread_create(&tid1,NULL,gen1,(void *)nums1);
		pthread_attr_init(&attr2);
		pthread_create(&tid2,NULL,gen2,(void *)nums2);
		pthread_join(tid1,NULL);
		pthread_join(tid2,NULL);
		fib=fib1+fib2;
		printf("%lld\n",fib);
	}
	return 0;
}
void *gen1(void *param)
{
	int * ar=(int *)param; int n = ar[0];
	long long int a = 0, b = 1, c, i; 
    if( n == 0) 
        fib1=a;
    else{
    for(i = 2; i <= n; i++) 
    { 
       c = a + b; 
       a = b; 
       b = c; 
    } 
    fib1=b;
   }
   pthread_exit(0);
}
void *gen2(void *param)
{
  int * ar=(int *)param; int n = ar[0];
	long long int a = 0, b = 1, c, i; 
    if( n == 0) 
        fib2=a;
    else{ 
    for(i = 2; i <= n; i++) 
    { 
       c = a + b; 
       a = b; 
       b = c; 
    } 
    fib2=b;
	}
	pthread_exit(0);
}