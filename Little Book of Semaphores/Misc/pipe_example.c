/*
* @Author: prateek
* @Date:   2020-11-30 22:54:32
* @Last Modified by:   prateek
* @Last Modified time: 2020-11-30 22:56:52
*/
#include <stdio.h>

int main()
{
    int fd[3],count=0;
    if(pipe(fd)!=0)
        perror("Pipe");
    count = write(fd[2],"OS2020",7);
    printf("%d\n",count);

}