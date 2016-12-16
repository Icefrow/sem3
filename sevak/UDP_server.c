#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NAME_LEN 100
#define MAX_NUM_OF_USERS 100

struct Adr{
    char name[MAX_NAME_LEN];
    struct sockaddr_in addr;
} typedef ADR;

ADR adress[MAX_NUM_OF_USERS];
int USER_COUNTER = 0;

void copy_adr(struct sockaddr_in *where, struct sockaddr_in *from);
int who_i_am(struct sockaddr_in cliaddr);


int main(){
    int sockfd;
    int clilen, n;
    int i;
    char line[1000];
    char finish_line[1000];
    int number_of_user;
    char user_name[MAX_NAME_LEN];
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)    {
        perror(NULL);
        exit(1);
    }
    else{
        printf("SERVER IS START\n");
    }
    if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    while (1){
        printf("USERS_LIST:\n");
        for (i = 0; i < USER_COUNTER; i++){
            printf("\t%s\n", adress[i].name);
        }
        clilen = sizeof(cliaddr);
        if ((n = recvfrom(sockfd, line, 1000, 0, (struct sockaddr*) (&cliaddr), (&clilen)) < 0)){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        printf("%s\n", line);
        if (line[0] == '$'){
            i = 1;
            while (line[i] != '\n')
                i++;
            strncpy(adress[USER_COUNTER].name, line + 1, i - 1);
            adress[USER_COUNTER].name[i] = '\0';
            copy_adr(&(adress[USER_COUNTER].addr), &cliaddr);
            strcpy(line, "Your name is registered\n");
            if (sendto(sockfd, line, strlen(line) + 1, 0, (struct sockaddr*)&(adress[USER_COUNTER].addr), clilen) < 0){
                perror(NULL);
                close(sockfd);
                exit(1);
            }
            USER_COUNTER++;
        }
        else{
            number_of_user = who_i_am(cliaddr);
            if (line[0] == '#'){
                i = 1;
                while (line[i] != ' ' && line[i] != '\n')
                    i++;
                user_name[0] = '\0';
                strncpy(user_name, line + 1, i-1);
                user_name[i-1] = '\0';
                strcpy(finish_line, adress[number_of_user].name);
                finish_line[strlen(adress[number_of_user].name)] = ':';
                finish_line[strlen(adress[number_of_user].name) + 1] = ' ';
                strcpy(finish_line + strlen(adress[number_of_user].name) + 2, line + i + 1);
                for (i = 0; i < USER_COUNTER; i++){
                    if (strcmp(user_name, adress[i].name) == 0){
                        if (sendto(sockfd, finish_line, strlen(finish_line) + 1, 0, (struct sockaddr*)&(adress[i].addr), clilen) < 0){
                            perror(NULL);
                            close(sockfd);
                            exit(1);
                        }
                        printf("\tSend to user %s \n", user_name);
                        goto NEXT;
                    }
                }
                strcpy(line, "\tIt is no user with this name\n");
                if (sendto(sockfd, line, strlen(line) + 1, 0, (struct sockaddr*)&cliaddr, clilen) < 0){
                    perror(NULL);
                    close(sockfd);
                    exit(1);
                }
            }
            else{
                strcpy(finish_line, adress[number_of_user].name);
                finish_line[strlen(adress[number_of_user].name)] = ':';
                finish_line[strlen(adress[number_of_user].name) + 1] = ' ';
                strcpy(finish_line + strlen(adress[number_of_user].name) + 2, line);
                printf("%s \n", line);
                for (i = 0; i < USER_COUNTER; i++){
                    if ((i != number_of_user) && (sendto(sockfd, finish_line, strlen(finish_line) + 1, 0, (struct sockaddr*)&(adress[i].addr), clilen) < 0)){
                        perror(NULL);
                        close(sockfd);
                        exit(1);
                    }
                }
                printf("\tSend to ALL users %s\n", line);
            }
        }
        NEXT: ;
    }
    return 0;
}

int who_i_am(struct sockaddr_in cliaddr){
    for (int i = 0; i < USER_COUNTER; i++){
        if ((cliaddr.sin_port == adress[i].addr.sin_port) && (cliaddr.sin_addr.s_addr == adress[i].addr.sin_addr.s_addr))
            return i;
    }
    return -1;
}

void copy_adr(struct sockaddr_in *where, struct sockaddr_in *from){
    where->sin_family = from->sin_family;
    where->sin_port = from->sin_port;
    where->sin_addr.s_addr = from->sin_addr.s_addr;
} 
