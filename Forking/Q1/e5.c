#include<unistd.h>
	#include <stdio.h>
	int main()
	{
	int pid;
	pid=fork(); //A
	if (pid > 0) {
	fork(); //B
	printf("OS \n");
	}
	printf("Hello \n");
	return 0;
	}
