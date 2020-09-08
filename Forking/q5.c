/*
* @Author: prateek
* @Date:   2020-08-31 18:10:22
* @Last Modified by:   prateek
* @Last Modified time: 2020-08-31 18:10:47
*/


#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>


int main ()
{
printf("A \n");
fork();
printf("B\n");
return 0;
}