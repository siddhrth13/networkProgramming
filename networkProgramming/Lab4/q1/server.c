#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/wait.h>

#define PORT 8080;
#define BUFFER_SIZE 1024;

struct student{
    char name[20];
    char regno[5];
    char marks[5];
};


int main(){

    struct student s1;
    strcpy(s1.name,"Rahul");
    strcpy(s1.marks,"90");
    strcpy(s1.regno,"1011");

    struct student s2;
    strcpy(s2.name,"Rohan");
    strcpy(s2.marks,"87");
    strcpy(s2.regno,"1013");
   

    struct student s3;
    strcpy(s3.name,"Sachin");
    strcpy(s3.marks,"81");
    strcpy(s3.regno,"1043");

    char buffer[BUFFER_SIZE];
    

    int server_fd,new_socket;
    struct sockaddr_in address;
    int addrlen=sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create Socket
    if((server_fd= socket(AF_INET,SOCK_STREAM,0))==0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    if(bind(server_fd,(struct sockaddr *)&address, sizeof(address))<0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }


    if(listen(server_fd,3)<0){
        perror("Listen Failure");
        exit(EXIT_FAILURE);
    }

    if((new_socket=accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
        perror("Accept Failure");
        exit(EXIT_FAILURE);
    }

    while(1){
        memset(buffer,0,sizeof(buffer));

        recv(new_socket,buffer,sizeof(buffer));
        
        
    }
}