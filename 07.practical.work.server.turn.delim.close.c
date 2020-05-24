#include <fcntl.h> 
#include <unistd.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#define MAX 80 
#define PORT 8784 
#define SA struct sockaddr 

char buff[MAX]; 

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
    int n;
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, sizeof(buff));
        n = recv(sockfd, buff, sizeof(buff), 0);
        int first = 0;
        if (strncmp("clexit",sizeof(buff)) == 0) {
            bzero(buff, sizeof(buff));
            buff[] = "svexit";
            send(sockfd, buff, 6, 0);
            shutdown(sockfd, WR);
            recv(sockfd, buff, 6, 0);
            if (strncmp("clexit",sizeof(buff)) == 0) 
                close(sockfd);
        } else {
            for (int i = 0; i < n; i++) {
                if(buff[i] == '\0') {
                    printf("Client : %s\n", buff[first]);
                    first = i + 1;
                }
            }
        }

        printf("Server : ");
        bzero(buff, sizeof(buff));
        scanf("%s", buff);
        if (strncmp("exit", buff, 4) == 0) {
            bzero(buff, sizeof(buff));
            buff[] ="svexit";
            send(sockfd, buff, 6, 0);
            recv(sockfd, buff, 6, 0);
            if (strncmp("clexit", buff, 6) == 0)
                close(sockfd); 
        } else
            send(sockfd, buff, strlen(buff) + 1, 0);  
        
    } 
} 

  
// Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
  
    // Function for chatting between client and server 
    func(connfd); 
  
}