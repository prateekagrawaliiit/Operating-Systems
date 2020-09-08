#include<stdio.h>
	#include<sys/types.h>
	#include<unistd.h>
	int main ()
	{ pid_t pid =fork();
	if(pid<0)
	printf("Fork Failed\n");
	else if(pid==0)
	printf("Child Block\n");
	else if(pid>0)
	printf("Parent Block\n");
	return 0;
	}
