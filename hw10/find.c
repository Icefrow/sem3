#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct dirent *ent;
char file_name[10] = "find.c";
char cpypath[100];
int a,b;
DIR *dir;

void DFS(char* path)
{
	ent = malloc(sizeof(struct dirent));
	dir = opendir(path);
	while( (ent = readdir(dir)) != NULL)
	{
		if(strcmp(ent->d_name,file_name) == 0)
		{
			printf("%s \n",path);
			exit(-1);
		}
		if( (ent->d_type == 4) && (strcmp(ent->d_name,".") != 0) && (strcmp(ent->d_name,"..") != 0) )
		{
			a = telldir(dir);
			strcpy(cpypath, path);
			strcat(path,ent->d_name);
			DFS(strcat(path,"/"));
			strcpy(path, cpypath);
			dir = opendir(cpypath);
			seekdir(dir,a);
		}
	}
	return;
}



int main()
{
	char path[255] = "../";
	DFS(path);
	return 0;
}

