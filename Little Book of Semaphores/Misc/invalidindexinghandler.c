//A code to demonstrate SIGSEGV and SIGILL handling using signals in POSIX
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<math.h>
int arr[] = {1,2,3,4,5}; 
void signal_1(int signum)
{
	printf("The last valid element entered is: %d\n",arr[4]);
	signal(SIGSEGV,SIG_DFL);
}
void signal_2(int signum)
{
	printf("The instruction is illlegal.\n");
	signal(SIGILL,SIG_DFL);
}
int main(int argc, char const *argv[])
{
	signal(SIGSEGV,signal_1);
	signal(SIGILL,signal_2);
	int arr[] = {1,2,3,4,5};
    arr[10] = 11; 
    raise(SIGILL);
	return 0;
}
