/*
* @Author: prateekagrawaliiit
* @Date:   2020-11-10 20:10:53
* @Last Modified by:   prateekagrawaliiit
* @Last Modified time: 2020-11-10 20:11:13
*/


#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	signal(SIGINT,SIG_IGN);
	for(int i=1;;i++)
	{
		printf("%d Inside Main function\n",i);
		sleep(1);

	}

	return 0;
}