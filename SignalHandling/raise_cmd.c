/*
* @Author: prateekagrawaliiit
* @Date:   2020-11-10 20:48:47
* @Last Modified by:   prateekagrawaliiit
* @Last Modified time: 2020-11-10 20:49:43
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signum)
{
	printf("Inside Signal Handler function\n");
}

int main(int argc,char* argv[])
{
	signal(SIGUSR1,sig_handler);
	printf("Inside main function\n");
	raise(SIGUSR1);
	printf("Inside main function\n");
}