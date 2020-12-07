/*
* @Author: prateek
* @Date:   2020-11-30 22:22:48
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-30 22:22:50
*/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int palindrome(char string1[])
{
    int length = strlen(string1);int flag=0;
    for(int i=0;i < length ;i++){
        if(string1[i] != string1[length-i-1]){
            flag = 1;
            break;
   }
}
return flag;
}
int main(int argc, char const *argv[])
{
	int file = open("List.txt",O_WRONLY | O_APPEND); 
    dup2(file,1);
    pid_t pid=fork();
    if(pid==0)
    {
	char command= "ls -1 | sed 's/\.[^.]$//'";
	system(command);
    close(file);
    exit(0);
    }
    else{
    wait(NULL);
    char buff[255];
    FILE *file1= fopen("List.txt","r");
    FILE *file2= fopen("Result.txt","w");
    while(fgets(buff, 255, (FILE*)file1))
    {
        buff[strcspn(buff, "\n")] = 0;
        if(palindrome(buff)==0)
        {
            fputs(buff,file2);
            fputs("\n",file2);
        }
    }
    fclose(file2);
    fclose(file1);
    }		
	return 0;
}