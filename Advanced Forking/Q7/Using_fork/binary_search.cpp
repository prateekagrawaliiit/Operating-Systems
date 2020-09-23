#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<bits/stdc++.h>
#include<string>
using namespace std;
int main(int argc, char *argv[])
{   
	int n;n= stoi(argv[1]);
	int key=stoi(argv[2]);
	int a[100];int j=0;
	for(int i=3;j<n;i++)
	{
		a[j]= stoi(argv[i]);
		j++;
	}
	sort(a,a+n/2);

	pid_t pid = fork();
	if(pid==0)
	{
		int count=0;
		int beg=(n/2)+1;
		int end=n;
		bool flag=false;
		while(beg<end)
		{
			int mid=(beg+end)/2;
			if(a[mid]==key)
			{
				count++;
				for(int i=mid+1;i<end;i++)
					{
						if(a[i]==key)
							count++;
						else
							break;
					}
				for(int i=beg;i<=mid;i++)
					{
						if(a[i]==key)
							count++;
						else
							break;
					}
			break;
			}
			else if(a[mid]<key)
				{
					beg=mid+1;
				}
			else
				end=mid-1;
		}
	printf("Occurences in second half = %d\n",count);
	}
	else if(pid >0)
	{	
		wait(NULL);
		int count=0;
		int beg=0;
		int end=n/2;
		bool flag=false;
		while(beg<=end)
		{
			int mid=(beg+end)/2;
			if(a[mid]==key)
			{
				count++;
				cout<<count;
				for(int i=mid+1;i<end;i++)
					{
						if(a[i]==key)
							count++;
						else
							break;
					}
				for(int i=beg;i<mid;i++)
					{
						if(a[i]==key)
							count++;
						else
							break;
					}
			break;
			}
			else if(a[mid]<key)
				{
					beg=mid+1;
				}
			else
				end=mid-1;
		}
	 printf("Occurences in first half = %d\n",count);	

	}
	else
		printf("FORKING FAILURE");

	_exit(0);
	return 0;

}

