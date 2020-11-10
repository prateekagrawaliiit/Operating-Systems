/*
* @Author: prateekagrawaliiit
* @Date:   2020-11-10 21:00:01
* @Last Modified by:   prateekagrawaliiit
* @Last Modified time: 2020-11-10 21:11:15
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void parent_handler(int signum)
{
	printf("Parent recieved a signal from the child\n");
}
void child_handler(int signum)
{
	printf("Child recieved a signal from the parent\n");
	sleep(1);
	pid_t pid = getppid();
	kill(pid, SIGUSR1);


}


int main(int argc, char* argv[])
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		printf("Forking failure\n");
		_exit(1);
	}
	else if (pid == 0)
	{
		signal(SIGUSR1, child_handler);
		printf("Child waiting for signal\n");
		pause();
	}
	else
	{
		signal(SIGUSR1, parent_handler);
		sleep(1);

		printf("Parent sending signal to child\n");

		kill(pid, SIGUSR1);
		printf("Parent waiting for response\n");
		pause();


	}

	return 0;
}