#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LEN_PARAMETERS 100 //длина каждой строки файла
#define  MAX_LETTERS_IN_WORD  20 //максильное кол-во букв в каждом слове
#define MAX_PARAMETERS 10 //кол-во параметров каждой строки
#define MAX_PROGRAMS 20 //кол-во программ

// Программа работает для любого файла с числом токенов (разделенные пробелом) каждой строки не больше 10

void zeroing (char ***s)
{
	int i;
	for(i = 0; i < MAX_PARAMETERS; i++)
		*(*s+i)=NULL;
}	


void split (char *str, char *delim, char ***words, int *n)
{
	int i = 0;
	*n = 0;
	char *lol;
	lol = strtok(str,delim);
	while(lol != NULL)
	{
		*(*words + i) = lol;
		lol = strtok(NULL,delim);
		++i;
		*n = *n + 1;
	}
}

int main()
{
	FILE *f = fopen("file.txt", "r");
	int i, j, n, x;
	int status;
/*	char **s = (char**) malloc(sizeof(char*) * MAX_LEN_PARAMETERS);
	почему то дампит, поэтому выделяю память способом ниже */ 
	char* s[MAX_PROGRAMS];

	for(i = 0; i < MAX_PROGRAMS; i++)
		s[i]=(char*) malloc(sizeof(char) * MAX_LEN_PARAMETERS);

	char **words = (char**) malloc(sizeof(char*) * MAX_LETTERS_IN_WORD);
	char a[10];
	
	n=atoi(fgets(a,10,f));
	
	for(i = 0;i < n; i++)
		fgets(s[i], sizeof(char) * MAX_LEN_PARAMETERS , f);

	for(i = 0; i < n; i++)
	{
		zeroing(&words);
		split(s[i]," \n", &words, &x);
		sleep(atoi(*words)); //из-за цикла слипаются по очереди
		pid_t t = fork();
		wait(&status);
		if(t == 0)
		{
			execlp(*(words+1),
			*(words+1),
			*(words+2),
			*(words+3),
			*(words+4),
			*(words+5),
			*(words+6),
			*(words+7),
			*(words+8),
			*(words+9),
			NULL);
			exit(0);
			
		}
	}
	return 0;
}
	

