#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
	int shmid;
	int fd;
	const char pathname[] = "config";
	key_t key;
	int new;
	char* array;
	
	
	key = ftok(pathname , 0);

	if((shmid = shmget(key, 100 * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if(errno != EEXIST) {
            printf("Can't create shared memory\n");
            exit(-1);
        } else {
         if((shmid = shmget(key, 100 * sizeof(char), 0)) < 0){
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
	
	printf("%s\n", array);
	if(shmdt(array) < 0) {
      printf("Can't detach shared memory\n");
      exit(-1);
   }
	shmctl(shmid, IPC_RMID, NULL);
	return 0;
}

