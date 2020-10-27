/*
* @Author: prateek
* @Date:   2020-10-26 19:26:46
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-26 19:27:25
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int num1,num2,sum;
	int input_fd = open("./input.txt",O_RDONLY);
	int output_fd = open("./output.txt",O_WRONLY);
	if(dup2(input_fd,STDIN_FILENO)<0)
	{
		printf("Unable to duplicate file descriptor\n");
		exit(EXIT_FAILURE);
	}
	if(dup2(output_fd,STDOUT_FILENO)<0)
	{
		printf("Unable to duplicate file descriptor\n");
		exit(EXIT_FAILURE);
	}
	scanf("%d %d",&num1,&num2);
	sum = num1+num2;
	// printf("%d\n",STDIN_FILENO);
	// printf("%d\n",STDOUT_FILENO);
	printf("%d + %d = %d\n",num1,num2,sum); 

}