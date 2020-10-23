/*
* @Author: prateek
* @Date:   2020-10-21 01:22:44
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-21 01:54:20
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
	char str1[6] = "Hello";
	char str3[10];
	char str4[5];
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
		r = read(fd2[READ_END], str4, sizeof(str4));
		int k = 0;
		for (int i = 0; i < 5; i++)
		{
			str3[i] = str1[i];
		}
		for (int j = 5; j < 10; j++)
		{
			str3[j] = str4[k];
			k++;
		}

		w = write(fd1[WRITE_END], str3, sizeof(str3)+1);

		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

	}

	else if (pid == 0)
	{
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);
		char str2[5] = "World";
		w = write(fd2[WRITE_END], str2, sizeof(str2) + 1);
		r = read(fd1[READ_END], str3, sizeof(str3));
		printf("String after concatination is :%s\n", str3);

		close(fd1[READ_END]);
		close(fd2[WRITE_END]);

	}
	else
	{
		fprintf(stderr, "Forking failure\n");
	}


}
