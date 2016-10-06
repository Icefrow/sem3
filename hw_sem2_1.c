#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void aba(int n,char* s)
{
	s[0]='a';
	int i;
	for(i = 1; i < n + 1; i++)
	{
		s[(1 << i) - 1]='a'+i;
		strncat(s + (1 << i) - 1, s, (1 << i));
	}
	s[(1 << n) - 1]='\0';
	//printf("%c\n", s[1]);
	printf("%s\n", s);
}

int main()
{
	int n;
	scanf("%d", &n);
	char *s = (char*) malloc(sizeof(char) * (1 << n));
	aba(n, s);
	return 0;
}
	
	



