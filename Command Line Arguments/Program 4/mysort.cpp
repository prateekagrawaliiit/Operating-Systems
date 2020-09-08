/*
* @Author: prateek
* @Date:   2020-08-30 17:44:59
* @Last Modified by:   prateek
* @Last Modified time: 2020-08-30 17:45:24
*/


// 4. Develop an application (using function templates & command line arguments in C) for:
// Same as above but you should define sort function only once internally and leave it to the
// compiler to generate data type specific functions. Clue is to use function templates feature in
// C.

#include<bits/stdc++.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
using namespace std;
// Template formed so that sorting of any
// type variable is possible
int strtoint(char *str)
{
	int i,x;
	for(x=0,i=0;str[i];i++)
	{
		if(i==0&& str[i]=='-')
			i++;
		if(str[i]>='0'&& str[i]<='9')
			x=x*10+(str[i]-48);
		else
			break;
	}
	if(str[0]=='-')
		x=-x;
	return(x);
}
double strtodouble(char *str)
{
	double x;
	x= atof(str);return(x);
}
template <class T>
void sort(T a[], int n, int flag)
{
// boolean variable to check that
// whether it is sorted or not
	bool b = true;
	if(flag==1){
		while (b) {
			b = false;
			for (size_t i=0; i<n-1; i++) {
// swapping the variable
// for sorting order
				if (a[i] > a[i + 1]) {
					T temp = a[i];
					a[i] = a[i + 1];
					a[i + 1] = temp;
					b = true;
				}
			} }}
			else{
				b =false;
				while(b){
					for (size_t i=0; i<n-1; i++) {
// swapping the variable
// for sorting order
						if (a[i] < a[i + 1]) {
							T temp = a[i];
							a[i] = a[i + 1];
							a[i + 1] = temp;
							b = true;
						}
					}}
				}
				for (size_t i = 0; i < n; ++i)
					cout << a[i] << " ";
				cout << endl;
			}
			int main(int argc, char *argv[])
			{
				if(argc<4)
					{printf("Wrong input format\n");
				return 0;
			}
			int n = strtoint(argv[1]);
			int flag= strtoint(argv[2]);
			printf("Size: %d\n",n );
			printf("A/D status: %d\n",flag );
			int a[100];
			double b[100];
			char c[100];
			int label=0;
			if(isdigit(argv[3][0]))
			{
				label=0;
				for(int i=0;i< strlen(argv[3]);i++)
					{ if(argv[3][i]=='.')
				{label=1; break;}}
				if(label==0)
				{
					int j=0;
					for(int i=3;j<n;i++)
					{
						a[j]= strtoint(argv[i]);
						j++;
					}
					sort(a,n,flag);
				}
				else
					{ int j=0;
						for(int i=3;j<n;i++)
						{
							b[j]= strtodouble(argv[i]);
							j++;
						}
						sort(b,n,flag);
					}
				}
				else if(isalpha(argv[3][0]))
				{
					int j=0;label=1;
					for(int i=3;j<n;i++)
					{
						c[j]= argv[i][0];j++;
					}
					sort(c,n,flag);
				}
				printf("\n");
				return 0;
			}