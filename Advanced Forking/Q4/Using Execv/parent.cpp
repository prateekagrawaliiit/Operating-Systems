/*
* @Author: prateek
* @Date:   2020-09-07 20:49:06
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-07 21:04:22
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
int main()
{
	int t1 = 1, t2 = 1, nextTerm;
	char n[]="";
	int num=0;
	cout<<"Enter a Number n : ";
	cin>>n;
	char *args[2] = {n,NULL};
	pid_t pid = fork();
	wait(NULL);
	if(pid==0)
	{
		execv("./child",args);
	}
	else if(pid>0)
	{
		cout<<"\nPrinting Fibonnaci Series"<<endl;
		num=stoi(n);
		for (int i = 1; i <= num; ++i) 
    	{
        printf("%d\n", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    	}

    	exit(0);
    	return 0;
	}
	else
	{
		printf("FORKING FAILURE");
	}

	exit(0);
	return 0;

}


