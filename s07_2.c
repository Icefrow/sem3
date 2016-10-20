#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define LETTERS 3000

int main()
{
	int shmid;
	int fd, a = 1;
	const char pathname[] = "config";
	key_t key;
	int new;
	char* array;
	
	
	key = ftok(pathname , 0);

	if((shmid = shmget(key, sizeof(char) * LETTERS, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if(errno != EEXIST) {
            printf("Can't create shared memory\n");
            exit(-1);
        } else {
         if((shmid = shmget(key, sizeof(char) * LETTERS, 0)) < 0){
            printf("Can't find shared memory\n");
            exit(-1);
         }
         new = 0;
        }
    }
	
	if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
      printf("Can't attach shared memory\n");
      exit(-1);
    }
	
	fd = open("s06_2.c",O_RDONLY);
	a = read(fd, array, LETTERS);
	
	if(shmdt(array) < 0) {
      printf("Can't detach shared memory\n");
      exit(-1);
   }
	return 0;
}
