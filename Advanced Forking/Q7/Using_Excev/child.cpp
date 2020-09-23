#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string>
#include<bits/stdc++.h>
using namespace std;
int main(int argc, char *argv[])
{   
	
  int count=0;
	int n= stoi(argv[0]);
	int a[100];int j=0;
	int key=stoi(argv[1]);

	for(int i=2;j<n;i++)
	{
		a[j]= stoi(argv[i]);
		j++;
	}

sort(a+n/2,a+n);
int beg=n/2;
int end=n;
bool flag=false;
while(beg<=end)
{
  int mid=(beg+end)/2;
  if(a[mid]==key)
  {
    count++;
    for(int i=mid+1;i<end;i++)
      if(a[i]==key)
        count++;
      else
        break;
    for(int i=beg;i<mid;i++)
      if(a[i]==key)
        count++;
      else
        break;
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



return 0;}