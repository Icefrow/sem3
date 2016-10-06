#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAX_STR  200
#define  MAX_DELIM  10
#define  MAX_LETTERS  20

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


int main () 
{
	int n=2,i;
    char *str = (char*) malloc(sizeof(char) * MAX_STR);
    char *delim = (char*) malloc(sizeof(char) * MAX_DELIM);
    char **words = (char**) malloc(sizeof(char*) * MAX_LETTERS);
	gets(str);
	gets(delim);

	split(str,delim,&words,&n);

	for(i = 0;i < n;i++)
		printf("%s\n",*(words + i));
  return 0;
}
