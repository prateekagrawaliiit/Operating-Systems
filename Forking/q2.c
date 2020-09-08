/*
* @Author: prateek
* @Date:   2020-08-31 14:16:23
* @Last Modified by:   prateek
* @Last Modified time: 2020-08-31 14:16:28
*/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
	pid_t pid;
	pid=fork();
	if (pid!=0)
		fork();
	fork();
	printf("Count \n");
	return 0;
}