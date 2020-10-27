/*
* @Author: prateek
* @Date:   2020-10-26 19:30:55
* @Last Modified by:   prateek
* @Last Modified time: 2020-10-26 19:35:28
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int file_desc = open("./dup2.txt",O_WRONLY|O_APPEND);
	if (file_desc<0)
		printf("Error in opening file\n");
	int copy_desc = dup(file_desc);
	write(copy_desc,"This is the output to the file named dup2.txt\n",46);
	write(file_desc,"This is the output to the file named dup2.txt\n",46);
	return 0;
}