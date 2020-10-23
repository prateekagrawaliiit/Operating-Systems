/*
* @Author: prateek
* @Date:   2020-10-21 00:39:25
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-21 01:33:12
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc,char* argv[])
{
	char write_message[10] = "Helloworld"; 
	char read_message[10];
	char reversed_message[10];

	int fd1[2],fd2[2];
	pid_t pid;
	if(pipe(fd1) == -1)
	{
		fprintf(stderr, "Some error occured\n");
		return 1;
	}
	if(pipe(fd2)==-1)
	{
		fprintf(stderr, "Some error occured\n");
		return 2;
	}

	pid = fork();
	if(pid>0)
	{

		close(fd1[READ_END]);
		close(fd2[WRITE_END]);
		write(fd1[WRITE_END],write_message,sizeof(write_message) +1);

		read(fd2[READ_END],read_message,sizeof(read_message));
		printf("Parent-->Value recieved by the child : %s\n",read_message);

		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

	}

	else if(pid==0)
	{

		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

		read(fd1[READ_END],read_message,sizeof(read_message));
		printf("Child--->Value recieved by the parent :%s\n",read_message);
		int j=0;
		int i=sizeof(read_message)/sizeof(read_message[0]) -1;
		for(;i>=0;i--)
		{
			reversed_message[j]=read_message[i];
			j++;
		}
		write(fd2[WRITE_END],reversed_message,sizeof(reversed_message)+1);

		close(fd1[READ_END]);
		close(fd2[WRITE_END]);

	}
	else
	{
		fprintf(stderr, "Forking failure\n");
	}


}
