/*
* @Author: prateek
* @Date:   2020-11-01 11:40:43
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-01 12:39:37
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{

	// Create file descriptors one for the child and one for the parent.
	int fd[2];

	// if there is an error in pipe creation exit and display error.
	if (pipe(fd) == -1)
	{
		printf("Error creating pipe. Exiting...\n");
		return 1;
	}

	// create two child process one to execture the ps command and other to find the ones starting with g
	pid_t pid1, pid2;
	if ((pid1 = fork()) < 0)
	{
		printf("First Fork failed. Exiting...\n");
		return 1;
	}

	// 
	if (pid1 == 0)
	{
		// make sure that the output of the ps command goes to the file and not onto the screen of the user.
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		// get the list of all active processes and send them through the pipe to the other child process.
		execlp("ps", "ps", "-A", NULL);

		close(fd[1]);
		close(fd[0]);
	}
	if ((pid2 = fork()) < 0)
	{
		printf("Second Fork failed. Exiting...\n");
		return 1;
	}
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		close(fd[0]);

		// read the processes from the pipe and select only the ones that start with g using the given options.. 
		execlp("grep", "grep", "-E", "\\bg", NULL);
		close(fd[0]);
		close(fd[1]);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return 0;
}