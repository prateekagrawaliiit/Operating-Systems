/*
* @Author: prateek
* @Date:   2020-10-21 22:23:01
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-21 23:01:29
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[])
{
	char str1[10] = "HelloWorld";
	int n=0,m=0;
	printf("The string is : %s . Please enter the starting and ending index accordingly.\n",str1);
	printf("Enter starting index :");
	scanf("%d",&n);
	printf("Enter ending index :");
	scanf("%d",&m);
	char str2[m-n];
	memset(str2,0,sizeof(str2));

	int r, w;

	int fd1[2], fd2[2];
	pid_t pid;
	if (pipe(fd1) == -1)
	{
		fprintf(stderr, "Some error occured\n");
		return 1;
	}
	if (pipe(fd2) == -1)
	{
		fprintf(stderr, "Some error occured\n");
		return 2;
	}
	pid = fork();
	if (pid > 0)
	{
		close(fd1[READ_END]);
		close(fd2[WRITE_END]);
		w = write(fd1[WRITE_END], str1, sizeof(str1)+1);
		r = read(fd2[READ_END],str2,sizeof(str2));
		printf("The substring is %s\n", str2);


		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

	}

	else if (pid == 0)
	{
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);
		r = read(fd1[READ_END], str1, sizeof(str1));
		for(int i=0;i<m-n;i++)
		{
			str2[i] = str1[n+i];
		}
		w = write(fd2[WRITE_END], str2, sizeof(str2) + 1);
		close(fd1[READ_END]);
		close(fd2[WRITE_END]);

	}
	else
	{
		fprintf(stderr, "Forking failure\n");
	}


}
