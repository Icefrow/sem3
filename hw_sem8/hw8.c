#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define TABLE_LIMIT 1
#define MAX_TYPES_OF_DISHES 20
#define MAX_LETTERS_OF_TYPE 20

struct sembuf mybuf;

void ChangeSem(int id, int x)
{
	mybuf.sem_op = x;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

	if (semop(id , &mybuf , 1) < 0) {
        printf("Can`t wait for condition\n");
        exit(-1);
    }
}	

struct dish
{
	char type[MAX_LETTERS_OF_TYPE];
	double time;
};


int main()
{
	int semid, status;
	const char pathname[] = "hw8.c";
	key_t key;

	int i = 0, j = 0, i2 = 0, count, COUNT_OF_TYPES;
	char washerType[MAX_LETTERS_OF_TYPE], wiperType[MAX_LETTERS_OF_TYPE];
	char end[6];
	int fd[2];
	FILE *washer,*wiper,*dishes;
	struct dish dishWasher[MAX_TYPES_OF_DISHES], dishWiper[MAX_TYPES_OF_DISHES];

	if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key\n");
        exit(-1);
    }

	if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }

	ChangeSem(semid, TABLE_LIMIT);

	washer = fopen("washer.txt","r");
	wiper = fopen("wiper.txt","r");
	dishes = fopen("dishes.txt","r");

	while( fscanf(washer, "%s %*c %lf\n", dishWasher[i].type, &dishWasher[i].time) != EOF)
		++i;

	COUNT_OF_TYPES = i;

	i = 0;
	while( fscanf(wiper, "%s %*c %lf\n", dishWiper[i].type, &dishWiper[i].time) != EOF)
		++i; 

	pipe(fd);
	pid_t t = fork();  
	if(t > 0)
	{
		while( fscanf(dishes, "%s %*c %d\n", washerType, &count) != EOF)
		{
			for(i = 0; i < COUNT_OF_TYPES; i++)
				if( strcmp(washerType, dishWasher[i].type) == 0 )
					break; 
			for(j = 0; j < count; j++)
			{
				ChangeSem(semid, -1);
				sleep(dishWasher[i].time);
				printf("Мойщик: помыл %s за %.1lf секунд\n", washerType, dishWasher[i].time);
				write(fd[1], washerType, MAX_LETTERS_OF_TYPE); 
			}
		}
		printf("Мойщик: я всё\n");
		write(fd[1], "end\0", 4);
		wait(&status);
	}

	if(t == 0)
	{
		read(fd[0], wiperType, MAX_LETTERS_OF_TYPE);
		while( strcmp(wiperType,"end\0") != 0)
		{
			for(i2 = 0; i2 < COUNT_OF_TYPES; i2++)
				if( strcmp(wiperType, dishWiper[i2].type) == 0 )
					break;
			sleep(dishWiper[i2].time);
			ChangeSem(semid, 1);
			printf("Протир: протер %s за %.1lf секунд\n", wiperType, dishWiper[i2].time);
			read(fd[0], wiperType, MAX_LETTERS_OF_TYPE); 
		}
		printf("Протир: я тоже всё\n");
	}
	semctl(semid, IPC_RMID, 0);
	
	return 0;
}
		  		


