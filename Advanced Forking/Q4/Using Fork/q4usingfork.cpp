/*
* @Author: prateek
* @Date:   2020-09-06 17:15:20
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-07 20:52:21
*/
#include <string>
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
using namespace std;
int main()
{
	int n=0,t1 = 1, t2 = 1, nextTerm; //range
	cout<<"Please enter a number n :";
	cin>>n;
	pid_t pid;
	pid = fork();
	wait(NULL);
	if(pid==0)
	{
   	 	printf("\nFibonacci Series:\n");
   	 	for (int i = 1; i <= n; ++i) 
    	{
        printf("%d\n", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    	}

	}
	else if(pid>0)
	{	   	 
		printf("\nPrime Series:\n");
		for(int i=2;i<=n;i++)
		{
			int count=0;
			for(int j=2;j<int(sqrt(i));j++)
				if(i%j==0)
					count++;
			if(count<1)
				cout<<i<<endl;


		}
	}
	else
	{
		cout<<"Forking Failure"<<endl;
	}

	return 0;

}