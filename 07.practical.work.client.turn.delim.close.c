#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netinet/in.h>
#include <fcntl.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#define MAX 80
#define PORT 8784
#define SA struct sockaddr 

char buff[MAX];

void send_msg(int sockfd,char buff[MAX]) {
    int l1 = strlen(buff) + 1;
    send(sockfd, buff, l1, 0);
}
  
void recv_msg(int sockfd) {
    char buff[MAX];
    memset(buff, 0, sizeof(buff));
    int n = recv(sockfd, buff, sizeof(buff), 0);
	printf("Server: %s\n", buff);
    if (strncmp("exit", buff, 4) == 0) { 
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);        
    }
}

int main() 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli;
    struct hostent *h;
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
    
    // assign IP, PORT 
    if ((h=gethostbyname("localhost")) == NULL) {
        printf("unknow host \n");
    }
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
    memcpy((char *) &servaddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } else
        printf("connected to the server..\n"); 


    // function for chat 
    while(1) {
        printf("Client : ");  
        memset(buff, 0, sizeof(buff));
        scanf("%s", buff);
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Client exit...\n"); 
            send_msg(sockfd, buff);
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd);
            break;
        } else       
        send_msg(sockfd, buff);

        memset(buff, 0, sizeof(buff));
        recv_msg(sockfd);
        if (strncmp("exit", buff, 4) == 0) {
    }

} 
