/*
* @Author: prateekagrawaliiit
* @Date:   2020-11-10 20:53:30
* @Last Modified by:   prateekagrawaliiit
* @Last Modified time: 2020-11-10 20:59:52
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signum)
{
	printf("Inside Signal Handler function\n");
}

int main(int argc, char* argv[])
{
	signal(SIGUSR1, sig_handler);
	pid_t pid = getpid();
	printf("Inside main function\n");
	kill(pid, SIGUSR1);
	printf("Inside main function\n");
}