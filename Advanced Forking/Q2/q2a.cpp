/*
* @Author: prateek
* @Date:   2020-09-06 16:35:55
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-06 16:41:51
*/
#include <string>
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
int main()
{
	int n=0; //range
	cout<<"Please enter a number n :";
	cin>>n;
	pid_t pid;
	pid = fork();

	if(pid==0)
	{
		for(int i=1;i<=n;i++)
			if(i%2==0)
				cout<<"Even : "<<i<<endl;
	}
	else if(pid>0)
	{
		for(int i=1;i<=n;i++)
			if(i%2==1)
				cout<<"Odd : "<<i<<endl;
	}
	else
	{
		cout<<"Forking Failure"<<endl;
	}

	return 0;

}