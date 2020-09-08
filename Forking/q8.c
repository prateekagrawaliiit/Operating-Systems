#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main ()
{
printf("Process 1 parent\n");
pid_t child_2, child_3;
child_2 = fork();
if (child_2 == 0)
{
//child 2 codes
printf("child_2\n");
pid_t child_4, child_5, child_6;
child_4=fork();
if(child_4==0)
{
//child 4 codes
printf("child_4\n");
pid_t child_8 =fork();
if(child_8==0)
printf("child_8\n"); //child 8 code
}
else{
child_5=fork();
if(child_5==0){
//child 5 codes
printf("child_5\n");pid_t child_9=fork();
if(child_9==0)
{
printf("child_9\n"); //child 9 code
}
}
else{
child_6=fork();
if(child_6==0){
//child 6 codes
printf("child_6\n"); //child 6 code
}
}
}
}
else
{
child_3 = fork();
if (child_3 == 0)
{
//child 3 codes
printf("child_3\n");
pid_t child_7;
child_7=fork();
if(child_7==0)
{
printf("child_7\n"); //child 7 code
}
}
}
return 0;}
