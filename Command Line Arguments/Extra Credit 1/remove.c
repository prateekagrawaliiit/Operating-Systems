#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>


int main(int argc, char* argv[])
{
  int status;
  if(argc != 2 || argv[1] == "--help")
  {
    printf("\nUsage: cpcmd source_file destination_file\n");
    exit(EXIT_FAILURE);
  }
 
  // char file_name[25];

  // printf("Enter name of a file you wish to delete\n");
  // gets(file_name);

  status = remove(argv[1]);

  if (status == 0)
    printf("%s file deleted successfully.\n", argv[1]);
  else
  {
    printf("Unable to delete the file\n");
    perror("Following error occurred");
  }

  exit(EXIT_SUCCESS);
}
