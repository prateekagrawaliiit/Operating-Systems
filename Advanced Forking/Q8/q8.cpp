#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <bits/stdc++.h>
using namespace std;
 int fib (int n) 
{ 
    
   if(n<=2)
   {
   	return 1;
   }
   else
   {
   	pid_t pid=vfork();             // shared memory forking
   	long int fib_0,fib_1;         
   	if(pid==0)
   	{
   		fib_0=fib(n-2);      //the child process calculates the value till n-2
   		_exit(0);
   	}
   	else if(pid>0)
   	{
   		fib_1=fib(n-1);      //the parent process calculates the value till n-1
   	}
   	wait(NULL);                    //waiting till we get the values
   	return fib_1+fib_0;
   }
} 

int main(int argc,char* argv[]) 
{
    int i, n;
    n=stoi(argv[1]);
    
    for(int i=1;i<=n;i++)
    {
    	cout<<fib(i)<<endl;
    }

    return 0;

}