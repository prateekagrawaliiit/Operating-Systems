#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define RD_END 0
#define WR_END 1
int main(int argc, char const *argv[])
{
	int n=argc-1;
	int status;
	int ar[n];
	for (int i = 0; i < n; ++i)
	{
		ar[i]= atoi(argv[i+1]);
	}
	int n1=n/2;
	int n2=n-n1; 
	int ar3[n];
	int ar1[n1],ar2[n2];
	for (int i = 0; i < n1; ++i)
	{
		ar1[i]=ar[i];
	}
	int p=0;
	for(int i=n/2;i<n;i++)
	{
		ar2[p]=ar[i];
		p++;
	}
	int pipefd1[2],pipefd2[2];
	int status1, status2;
	status1= pipe(pipefd1);
	if(status1==-1)
	{
		printf("Pipe Failed\n");
		return 0;
	}
	status2= pipe(pipefd2);
	if(status2==-1)
	{
		printf("Pipe Failed\n");
		return 0;
	}
	if (fork()==0)
	{
		for (int i = 0; i < n1; ++i)
		{
			for (int j= i+1; j < n1; ++j)
			{
				if(ar1[i]>ar1[j])
				{
					int temp=ar1[i];
					ar1[i]=ar1[j];
					ar1[j]=temp;
				}
			}
		}
		printf("First list sorted\n");
		close(pipefd1[RD_END]);
		write(pipefd1[WR_END],&ar1,sizeof(ar1));
		exit(0);
	}
	
	if(fork()==0)
	{	
		for (int i = 0; i < n2; ++i)
		{
			for (int j= i+1; j < n2; ++j)
			{
				if(ar2[i]>ar2[j])
				{
					int temp=ar2[i];
					ar2[i]=ar2[j];
					ar2[j]=temp;
				}
			}
		}
		printf("Second list sorted\n");
		close(pipefd2[RD_END]);
		write(pipefd2[WR_END],&ar2,sizeof(ar2));
		exit(0);

	}

	if(fork()==0)
		{	
			wait(NULL);
			close(pipefd1[WR_END]);close(pipefd2[WR_END]);
			read(pipefd1[RD_END],&ar4,sizeof(ar4));
			printf("The third process received this as the first list: \n");
			for (int i = 0; i < n1; ++i)
    		{
    			printf("%d\n",ar4[i]);
    		}
			read(pipefd2[RD_END],&ar5,sizeof(ar5));
			printf("The third process received this as the second list: \n");
			for (int i = 0; i < n2; ++i)
    		{
    			printf("%d\n",ar5[i]);
    		}
    		printf("Merging the obtained lists\n");
    		int i = 0, j = 0, k = 0; 
  			while (i<n1 && j <n2) 
    		{	 
        		if (ar4[i] < ar5[j]) 
            		ar3[k++] = ar4[i++]; 
       			else
            		ar3[k++] = ar5[j++]; 
    		} 
  
    		while (i < n1) 
        		ar3[k++] = ar4[i++]; 
 
    		while (j < n2) 
        		ar3[k++] = ar5[j++]; 
    		for (int l = 0; l < n; ++l)
    		{
    			printf("%d\n",ar3[l]);
    		}
			exit(0);
		}
}