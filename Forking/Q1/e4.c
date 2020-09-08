#include<unistd.h>
	#include <stdio.h>
	int main()
	{
	int pid;
	pid=fork(); //A
	if (pid<0) fprintf(stderr,"failed fork \n");
	else if (pid==0)
	{
	fork(); //B
	printf("child print \n");
	}
	else if (pid>0)
	printf("Parent Print \n");
	printf("Main Print \n");
	return 0;
	}
