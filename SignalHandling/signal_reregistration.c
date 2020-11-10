/*
* @Author: prateekagrawaliiit
* @Date:   2020-11-10 20:11:49
* @Last Modified by:   prateekagrawaliiit
* @Last Modified time: 2020-11-10 20:12:51
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signum)
{
	printf("Inside Signal Handler function\n");
	signal(SIGINT, SIG_DFL);
}

int main(int argc, char* argv[])
{
	signal(SIGINT, sig_handler);
	for (int i = 1;; i++)
	{
		printf("%d Inside Main function\n", i);
		sleep(1);

	}

	return 0;
}