#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
 
#define BUFF_SIZE 1024
 
int main(int argc, char* argv[])
{
	int srcFD,destFD,nbread,nbwrite;
	char *buff[BUFF_SIZE];
	
	/*Check if both src & dest files are received or --help is received to get usage*/
	if(argc != 3 || argv[1] == "--help")
	{
		printf("\nUsage: cpcmd source_file destination_file\n");
		exit(EXIT_FAILURE);
	}
 
	/*Open source file in read mode*/
	srcFD = open(argv[1],O_RDONLY);
 
	if(srcFD == -1)
	{
		printf("\nError opening file %s errno = %d\n",argv[1],errno);
		exit(EXIT_FAILURE);	
	}
	
	/*Open destination file with respective flags & modes
	  O_CREAT & O_TRUNC is to truncate existing file or create a new file
	  S_IXXXX are file permissions for the user,groups & others*/
	destFD = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
 
	if(destFD == -1)
	{
		printf("\nError opening file %s errno = %d\n",argv[2],errno);
		exit(EXIT_FAILURE);
	}
 
	/*Start data transfer from src file to dest file till it reaches EOF*/
	while((nbread = read(srcFD,buff,BUFF_SIZE)) > 0)
	{
		if(write(destFD,buff,nbread) != nbread)
			printf("\nError in writing data to %s\n",argv[2]);
	}
	
	if(nbread == -1)
		printf("\nError in reading data from %s\n",argv[1]);
	
	if(close(srcFD) == -1)
		printf("\nError in closing file %s\n",argv[1]);
 
	if(close(destFD) == -1)
		printf("\nError in closing file %s\n",argv[2]);
 
	exit(EXIT_SUCCESS);
}