#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#define MAX_SPECTATORS 100
#define MAX_IMMIGRANTS 100

struct data
{
	int i;
};
// -----------------------------------------------------------
int n_imm, n_spec, n_jud;
int waiting[MAX_IMMIGRANTS];

int n_imm_checkIn = 0;
int n_imm_swear = 0;
sem_t mutex_imm_checkin;
sem_t mutex_imm_swear;
sem_t enterBlock;

int judge_entered = 0;
int confirmed = 0;
// -----------------------------------------------------------

// -----------------------------------------------------------
// SPECTATOR
void spec_enter(int i)
{
	sem_wait(&enterBlock);	// Check if allowed to enter
	sem_post(&enterBlock);	// Allow others to enter

	printf("Spectator %d has entered.\n", i);
}
void spec_spectate(int i)
{
	printf("Spectator %d is spectating.\n", i);
}
void spec_leave(int i)
{
	printf("Spectator %d has exited.\n", i);
}
void * spectator(void * d)
{
	struct data *p = (struct data *)d;
	do
	{
		spec_enter(p->i);
		spec_spectate(p->i);
		spec_leave(p->i);

	} while(1);

	pthread_exit(0);
}
// SPECTATOR

// JUDGE
void jud_enter(int i)
{
	sem_wait(&enterBlock);
	printf("Judge %d has entered.\n", i);
	judge_entered = 1;
}
void jud_confirm(int i)
{
	while(n_imm_checkIn < n_imm);// Wait till all immigrants are checked in

	while(n_imm_swear < n_imm)
	{
		printf("Judge %d confirmed immigrant.\n", i);
		confirmed = 1;			// Inform immigrant of confirmation
		while(confirmed == 1);	// Wait till immigrant accepts confirmation
	}
	
}
void jud_leave(int i)
{
	printf("Judge %d has exited.\n", i);
	n_imm_checkIn = 0;
	n_imm_swear = 0;
	judge_entered = 0;
	sem_post(&enterBlock);
}
void * judge(void * d)
{
	struct data *p = (struct data *)d;
	do
	{
		jud_enter(p->i);
		jud_confirm(p->i);
		jud_leave(p->i);

	} while(1);

	pthread_exit(0);
}
// JUDGE

// IMMIGRANT
void imm_enter(int i)
{
	sem_wait(&enterBlock);	// Check if u can enter
	sem_post(&enterBlock);
	waiting[i] = 1;
	printf("Immigrant %d is waiting.\n", i);
}
void imm_checkIn(int i)
{
	sem_wait(&mutex_imm_checkin);
	n_imm_checkIn++;
	printf("Immigrant %d has checkedIn.\n", i);
	sem_post(&mutex_imm_checkin);
}
void imm_sitDown(int i)
{
	printf("Immigrant %d sat down.\n", i);
}
void imm_swear(int i)
{
	while(judge_entered == 0);
	sem_wait(&mutex_imm_swear);
	printf("Immigrant %d sweared.\n", i);
}
void imm_getCertificate(int i)
{
	while(confirmed == 0);	// Wait till confirmation
	confirmed = 0;			// Acknowledge confirmation
	n_imm_swear++;
	printf("Immigrant %d accepted certificate.\n", i);
	sem_post(&mutex_imm_swear);
}
void imm_leave(int i)
{
	waiting[i] = 0;
	printf("Immigrant %d has exited.\n", i);
}
void * immigrant(void * d)
{
	struct data *p = (struct data *)d;
	do
	{
		imm_enter(p->i);
		imm_checkIn(p->i);
		imm_sitDown(p->i);
		imm_swear(p->i);
		imm_getCertificate(p->i);
		imm_leave(p->i);

	} while(1);

	pthread_exit(0);
}
// IMMIGRANT
// -----------------------------------------------------------

int main(int argc, char *argv[])
{
	//printf("------------------------------------ Faneuil Hall --------------------------------------\n");

	if(argc != 3) printf("Wrong number of parameters.\n");
	else
	{
		n_imm = atoi(argv[1]);
		n_spec = atoi(argv[2]);
		n_jud = 1;

		sem_init(&mutex_imm_checkin, 0, 1);
		sem_init(&mutex_imm_swear, 0, 1);
		sem_init(&enterBlock, 0, 1);
		for(int i=0;i<n_imm;i++)
		{
			waiting[i] = 0;
		}

		pthread_t tid_imm[n_imm];
		pthread_t tid_spec[n_spec];
		pthread_t tid_jud[n_jud];

		pthread_attr_t attr;
		pthread_attr_init(&attr);

		struct data d_imm[n_imm];
		struct data d_spec[n_spec];
		struct data d_jud[n_jud];

		for(int i=0;i<n_imm;i++)
		{
			d_imm[i].i = i;
			pthread_create(&tid_imm[i], &attr, immigrant, &d_imm[i]);
		}
		for(int i=0;i<n_spec;i++)
		{
			d_spec[i].i = i;
			pthread_create(&tid_spec[i], &attr, spectator, &d_spec[i]);
		}
		for(int i=0;i<n_jud;i++)
		{
			d_jud[i].i = i;
			pthread_create(&tid_jud[i], &attr, judge, &d_jud[i]);
		}

		for(int i=0;i<n_imm;i++)
		{
			pthread_join(tid_spec[i], NULL);
		}
		for(int i=0;i<n_imm;i++)
		{
			pthread_join(tid_imm[i], NULL);
		}
		for(int i=0;i<n_imm;i++)
		{
			pthread_join(tid_jud[i], NULL);
		}
	}
	printf("\n");
	//printf("------------------------------------ Faneuil Hall --------------------------------------\n");
	return 0;
}
