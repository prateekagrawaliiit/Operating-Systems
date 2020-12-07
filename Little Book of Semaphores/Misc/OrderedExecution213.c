//3 threads have to be synchronised in such a way, that the order is 2->1->3 using binary semaphores.
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
int s1=0,s2=0;
int signal(int s)
{
	s++;
	return s;
}
void *gen1(void *params)
{
	while(s1==0)
	{
		if(s1>0)
			break;
	}
	printf("Process 1\n");
	//sleep(3);
	s2=signal(s2);
	pthread_exit(0);
}
void *gen2(void *params)
{
	printf("Process 2\n");
	//sleep(3);
	s1=signal(s1);
	pthread_exit(0);
}
void *gen3(void *params)
{
	while(s2==0)
	{
		if(s2>0)
			break;
	}
	printf("Process 3\n");
	//sleep(3);
	pthread_exit(0);
}
int main(int argc, char const *argv[])
{
	pthread_t tid[3];
	pthread_create(&tid[0],NULL,gen1,(void *)NULL);
	pthread_create(&tid[1],NULL,gen2,(void *)NULL);
	pthread_create(&tid[2],NULL,gen3,(void *)NULL);
	int i=0;
	while(i<3)
	{
		pthread_join(tid[i],NULL);i++;
	}
	return 0;
}