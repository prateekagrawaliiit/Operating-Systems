/*
* @Author: prateek
* @Date:   2020-10-27 20:02:49
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-27 20:11:31
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h> 

#define MAX 512

int main(int argc, char* argv[])
{
    int fd[2];
    char buf[MAX];
    int nb, i;

    if (pipe(fd) == -1)
    {
        perror("Creating pipe");
        exit(1);
    }
    switch (fork())
    {
    case -1:
        perror("Creating a process");
    case 0:
        dup2(fd[1], 1);
        execvp("ls", argv);
        perror("Program ls");
        exit(1);
    default:
        close(fd[1]);
        while ((nb = read(fd[0], buf, MAX)) > 0)
        {
            for (int i = 0; i < nb; i++)
                buf[i] = toupper(buf[i]);
            if (write(1, buf, nb) == -1)
            {
                perror("Writing to stdout");
                exit(3);

            }
            if (nb == -1)
            {
                perror("Creating a process");
                exit(4);
            }

            exit(1);


        }



    }

    return 0;
}