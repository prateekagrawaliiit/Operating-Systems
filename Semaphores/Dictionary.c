//Setting up a dictionary simulated using readers writers problem.
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
sem_t wrt; char search[100];
pthread_mutex_t mutex;
int numreader = 0;int k=0;int flag;

typedef struct 
{
	char word[100];
	char primary[1000];
	char secondary[100];
} dict;

dict dictionary[5];
void *writer(void *wno)
{   
    sem_wait(&wrt);
    flag=1;
    printf("Enter word:\n");
    scanf("%s",search);
    int i;
    for(i=0;i<5;i++)//duplicacy check
    {
    	if(strcmp(dictionary[i].word,search)==0)
    	{
    		printf("Word already present.\n");
    		flag=0;
    		break;
    	}
    }
    if(flag==1)
    {
    strcpy(dictionary[k].word,search);
    printf("Enter meaning:\n");
    scanf("%s",dictionary[k].primary);
    printf("Enter secondary meaning:\n");
    scanf("%s",dictionary[k].secondary);
    printf("Writer added a word %s.\n",dictionary[k].word);k++;
	}
    sem_post(&wrt);
}
void *reader(void *rno)
{   
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); // If this id the first reader, then it will block the writer
    }
    pthread_mutex_unlock(&mutex);
    // Reading Section
    int i;
    printf("Enter word you wanna search: \n");
    scanf("%s",search);
    int low=0;
 	int high=4;
 	while(low<=high)
 	{
 		int mid=(low+high)/2;
 		if (strcmp(search,dictionary[mid].word)==0)
 		{
 			printf("Meaning: %s\n", dictionary[mid].primary);
 			printf("Secondary Meaning: %s\n", dictionary[mid].secondary);
        	exit(0);
 		}
 		else if(strcmp(search,dictionary[mid].word)>0)
 		{
 			high=high;
 			low=mid+1;
 		}
 		else
 		{
 			low=low;
 			high=mid-1;
 		}
 	}
 	printf("Word not found\n");
    // Reader acquire the lock before modifying numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) 
    {
        sem_post(&wrt); // If this is the last reader, it will wake up the writer.
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{   

    pthread_t read,write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);
    for(int i = 0; i < 5; i++) {
        pthread_create(&write[i], NULL, (void *)writer, NULL);
    }

   	pthread_create(&read, NULL, (void *)reader, NULL);  
    pthread_join(read, NULL);
    
    for(int i = 0; i < 5; i++) {
        pthread_join(write[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
    
}