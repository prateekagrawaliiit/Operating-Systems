/*
* @Author: prateekagrawaliiit
* @Date:   2020-11-10 19:54:30
* @Last Modified by:   prateekagrawaliiit
* @Last Modified time: 2020-11-10 19:56:13
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
	signal(SIGINT,sig_handler);
	for(int i=1;;i++)
	{
		printf("%d Inside Main function\n",i);
		sleep(1);

	}

	return 0;
}