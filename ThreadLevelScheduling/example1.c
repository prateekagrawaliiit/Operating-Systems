/*
* @Author: prateek
* @Date:   2020-11-10 15:15:35
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-10 15:23:01
*/
#include <stdio.h>
#include <pthread.h>

#define N 5

void *runner(void *params);


int main(int argc, char* argv[])
{
	int i, scope;
	pthread_t tid[N];
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	if (pthread_attr_getscope(&attr, &scope) != 0)
	{
		fprintf(stderr, "Unable to get scheduling scope\n");
	}
	else
	{
		if (scope == PTHREAD_SCOPE_PROCESS)
		{
			printf("PTHREAD_SCOPE_PROCESS\n");
		}
		else if (scope == PTHREAD_SCOPE_SYSTEM)
		{
			printf("PTHREAD_SCOPE_SYSTEM\n");
		}
		else
		{
			fprintf(stderr, "Illegal scope value\n");
		}
	}

	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);

	for(i=0;i<N;i++)
		pthread_create(&tid[i],&attr,runner,NULL);

	for(i=0;i<N;i++)
		pthread_join(tid[i],NULL);

}

void *runner(void *params)
{
	// printf("Hello\n");
	pthread_exit(0);

}