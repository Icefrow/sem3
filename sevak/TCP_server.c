#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int COUNT_OF_CLIENTS = 0;
int clientfd[100];

void* my_thread(int* fd)
{
	int n,i;
	char msg[1000];
	while((n = read(*fd, msg, 999)) > 0){
		printf("%s", msg);	
		for(i = 0; i < COUNT_OF_CLIENTS; i++)
			if(clientfd[i] != *fd)
				if((n = write(clientfd[i], msg, strlen(msg)+1)) < 0){
					perror(NULL);
					close(*fd);
					exit(1);
				}
	}
    if(n < 0){
            perror(NULL);
            close(*fd);
            exit(1);
        }
	return NULL;	
}

int main()
{
    int sockfd;
    unsigned int clilen; 
    int t[100];
    pthread_t thread_id[100]; 
    struct sockaddr_in servaddr, cliaddr; 
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family= AF_INET;
    servaddr.sin_port= htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    if(listen(sockfd, 5) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    while(1){
        clilen = sizeof(cliaddr);
        if((clientfd[COUNT_OF_CLIENTS] = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        t[COUNT_OF_CLIENTS] = pthread_create(&thread_id[COUNT_OF_CLIENTS], (pthread_attr_t *)NULL, my_thread, (int*)&clientfd[COUNT_OF_CLIENTS]);
        COUNT_OF_CLIENTS++;
    }
}
