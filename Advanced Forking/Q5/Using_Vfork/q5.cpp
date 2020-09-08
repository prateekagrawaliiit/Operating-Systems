/*
* @Author: prateek
* @Date:   2020-09-08 15:12:39
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-08 15:18:50
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int main()
{
	int n=0;
	cout<<"Enter the number of elements in the array :"<<endl;
	cin>>n;
	int arr[n];
	for(int j=0;j<n;j++)
		cin>>arr[j];
	pid_t pid = fork();
	wait(NULL);
	if(pid==0)
	{
		for (int i = 0; i < n; i++)                 //Loop for ascending ordering
		{
   			 for (int j = 0; j < n; j++)             //Loop for comparing other values
    		{
     			if (arr[j] > arr[i])                      //Comparing other array elements
     			{
       				int tmp = arr[i];                     //Using temporary variable for storing last value
        			arr[i] = arr[j];                        //replacing value
        			arr[j] = tmp;                         //storing last value
    			}  
			}
		}
  		printf("Ascending :\n");                     //Printing message
  		for (int i = 0; i < n; i++)                     //Loop for printing array data after sorting
  		{
  		printf(" %d ", arr[i]);
  		}
	  	printf("\n");
	}
	else if(pid<0)
	{
		printf("FORKING FAILURE");
	}
	else
	{
		for (int i = 0; i < n; i++)                 //Loop for ascending ordering
		{
   			 for (int j = 0; j < n; j++)             //Loop for comparing other values
    		{
     			if (arr[j] < arr[i])                      //Comparing other array elements
     			{
       				int tmp = arr[i];                     //Using temporary variable for storing last value
        			arr[i] = arr[j];                        //replacing value
        			arr[j] = tmp;                         //storing last value
    			}  
			}
		}
  		printf("Descending :\n");                     //Printing message
  		for (int i = 0; i < n; i++)                     //Loop for printing array data after sorting
  		{
  		printf(" %d ", arr[i]);
  		}
	  	printf("\n");



	}
}


