/*
* @Author: prateek
* @Date:   2020-09-07 20:44:32
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-07 21:02:28
*/
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
using namespace std;
int main(int argc,char* argv[])
{
	int n = stoi(argv[0]);
	printf("\nPrime Series:\n");
	for(int i=2;i<=n;i++)
	{
		int count=0;
		for(int j=2;j<=int(sqrt(i));j++)
			if(i%j==0)
				count++;
			if(count<1)
				cout<<i<<endl;
	}
	_exit(0);
	return 0;


}


