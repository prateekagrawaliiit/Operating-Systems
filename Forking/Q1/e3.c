
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>	
int main()
	{
	int pid;
	pid = fork();
	if (pid < 0)
	printf("Fork failed\n");
	else if (pid == 0)
	printf("Child block\n");
	else if (pid > 0)
	printf("Parent block\n");
	printf("Welcome to OS\n");
	return 0;
	}
