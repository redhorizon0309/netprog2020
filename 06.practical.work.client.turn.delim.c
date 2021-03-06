#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netinet/in.h>

#define MAX 80
#define PORT 8784
#define SA struct sockaddr 


void func(int sockfd) 
{ 
    char buff[MAX];
    int n; 
    for (;;) { 
        
        printf("Enter message : ");  
        
        bzero(buff, sizeof(buff));
        fgets(buff,MAX,STDIN);
        int l1 = strlen(buff) +1;
        n = send(sockfd, buff, l1, 0);
             

        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        }  

		memset(buff, 0, sizeof(buff));
		recv(sockfd, buff, sizeof(buff), 0);
		printf("Server: %s\n", buff);

        
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
    if ((h=gethostbyname("127.0.0.1")) == NULL) {
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
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    func(connfd); 
  
    // close the socket 
    close(connfd); 
} 