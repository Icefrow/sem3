#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define N 4 //number of forks

int main()
{
	int i, status;
	for(i=0;i<N;i++)
	{
		pid_t pid=fork();
		wait(&status);
		if(pid > 0)
		{
			printf("%d %d %d\n",getpid(),getppid(),1);
			exit(0);
		}
	}
	return 0;
}
