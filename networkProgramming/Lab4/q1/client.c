#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){

    int sock=0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char name[BUFFER_SIZE];

    if((sock=socket(AF_INET,SOCK_STREAM,0))<0){
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
        perror("Invalid address/address not sumpported");
        exit(EXIT_FAILURE);
    }

    while(1){

        memset(buffer,0,sizeof(buffer));
        memset(name,0,sizeof(name));

        printf("\nEnter name of the student: ");
        scanf("%s",&name);

        send()
        
    }

}