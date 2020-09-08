/*
* @Author: prateek
* @Date:   2020-09-08 15:30:41
* @Last Modified by:   prateek
* @Last Modified time: 2020-09-08 15:35:14
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{   
	

	int n= stoi(argv[0]);
	int a[100];int j=0;

	for(int i=1;j<n;i++)
	{
		a[j]= stoi(argv[i]);
		j++;
	}
	
	for (int i = 0; i < n; i++)                     //Loop for descending ordering
	{
		for (int j = 0; j < n; j++)             //Loop for comparing other values
		{
			if (a[j] < a[i])                //Comparing other array elements
			{
				int tmp = a[i];         //Using temporary variable for storing last value
				a[i] = a[j];            //replacing value
				a[j] = tmp;             //storing last value
			}
		}
	}
	printf("Descending :\n ");                    //Printing message
	for (int i = 0; i < n; i++)                     //Loop for printing array data after sorting
	{
		printf("%d ", a[i]);                   //Printing data
	}

    printf("\n");
	return 0;
}


