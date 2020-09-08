/*
* @Author: prateek
* @Date:   2020-09-06 16:45:03
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-06 16:53:23
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
	int status;
	for(int i=1;i<=n;i++)
	{
		int sum=0,k=i;
		
		pid = fork();
		if(pid==0)
		{	int num=0;
			while(k>0)
			{
				num=k%10;
				k=k/10;
				sum += num*num*num;

			}


		}
		else if(pid>0)
		{	
			pid = waitpid(pid, &status, 0);
		}
		
		else
			cout<<"Forking Failure"<<endl;
		if(sum==i)
				cout<<"This is an Armstrong Number : "<<i<<endl;


	}





}