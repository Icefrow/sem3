#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int sockfd, t; 
	int n;  
    char sendmsg[1000],getmsg[1000]; 
	struct sockaddr_in servaddr; 
	if(argc != 2){
		printf("Usage: a.out <IP address>\n");
		exit(1);
	}
    bzero(sendmsg,1000);
    bzero(getmsg,1000);
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL); 
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
        printf("Invalid IP address\n");
        close(sockfd);
        exit(1);
    }
    if(connect(sockfd, (struct sockaddr *) &servaddr, 
    sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    t = fork();
    if(t){
		while(1){
			fflush(stdin);
			fgets(sendmsg, 1000, stdin);
			if( (n = write(sockfd, sendmsg, strlen(sendmsg)+1)) < 0){
				perror("Can\'t write\n");
				close(sockfd);
				exit(1);
			}
		}
	}else{
		while(1){
			if ( (n = read(sockfd,getmsg, 1000)) < 0){
				perror("Can\'t read\n");
				close(sockfd);
				exit(1);
			}
			printf("%s", getmsg);
		}
    }
    close(sockfd);
}
