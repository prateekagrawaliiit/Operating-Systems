/*
* @Author: prateek
* @Date:   2020-10-27 22:36:40
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-27 22:41:03
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX 512

int main()
{
    char *cmd1[] = {"/bin/ls", "-al", "/", 0};
    char *cmd2[] = {"/usr/bin/tr", "a-z", "A-Z", 0};
    int pid;
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("Creating pipe");
        exit(1);
    }
    switch (pid = fork())
    {
    case 0:
        dup2(fd[0], 0);
        close(fd[1]);
        execvp(cmd2[0], cmd2);
        perror(cmd2[0]);
    case -1:
        perror("Creating a process");
        exit(3);
    default:
        dup2(fd[1], 1);
        close(fd[0]);
        execvp(cmd1[0], cmd1);
        perror(cmd1[0]);
    }



    return 0;
}