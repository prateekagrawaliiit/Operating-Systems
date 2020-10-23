/*
* @Author: prateek
* @Date:   2020-10-21 23:02:23
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-21 23:27:14
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
	char str1[7] = "ABCDCBA";
	char str2[7];
	char str3[7];


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
		r = read(fd2[READ_END], str2, sizeof(str2));
		printf("The reversed string by the child is '%s'\n", str2);

		int flag = 1;
		//printf("%ld	%s	%s", strlen(str1), str1, str2);
		for (int i = 0; i < strlen(str2); i++)
		{
			if (str2[i] != str1[i])
			{
				flag = 0;
				break;
			}
		}

		if (flag == 0)
		{
			printf("It's not a Palindrome\n");
		}
		else
		{
			printf("It's a Palindrome\n");
		}

		close(fd1[WRITE_END]);
		close(fd2[READ_END]);

	}

	else if (pid == 0)
	{
		close(fd1[WRITE_END]);
		close(fd2[READ_END]);
		r = read(fd1[READ_END], str2, sizeof(str2));
		int j = 0;
		int i = sizeof(str2) / sizeof(str2[0]) - 1;
		for (; i >= 0; i--)
		{
			str3[j] = str2[i];
			j++;
		}
		write(fd2[WRITE_END], str3, sizeof(str3) + 1);
	}
	else
	{
		fprintf(stderr, "Forking failure\n");
	}


}
