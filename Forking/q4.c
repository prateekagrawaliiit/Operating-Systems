/*
* @Author: prateek
* @Date:   2020-08-31 18:08:16
* @Last Modified by:   prateek
* @Last Modified time: 2020-08-31 18:09:13
*/


#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main ()
{
printf("This will be printed ?.\n");
fork();
printf("This will be printed ?.\n");
fork();
printf("This will be printed ?.\n");
fork();
printf("This will be printed ?.\n");
return 0;
}