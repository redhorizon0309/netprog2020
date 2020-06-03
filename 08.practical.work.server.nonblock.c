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

void send_msg(int sockfd,char buff[MAX]) {
    int n = strlen(buff) + 1;
    send(sockfd, buff, n, 0);
}
  
void recv_msg(int sockfd) {
    char buff[MAX];
    memset(buff, 0, sizeof(buff));
    int n = recv(sockfd, buff, sizeof(buff), 0);
    printf("Client: %s\n", buff);
    if (strncmp("exit", buff, 4) == 0) {
        send(sockfd, buff, 5, 0);
        close(sockfd);
    }    
}

  
// Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    int fl = fcntl(sockfd, F_GETFL, 0);
    fl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, fl);

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) < 0) { 
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
  
    
LOOP:   while(1) {
            connfd = accept(sockfd, (SA*)&cli, &len); 
            if (connfd > 0) {
                printf("server acccept the client...\n"); 
                setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
                int fl = fcntl(connfd, F_GETFL, 0);
                fl |= O_NONBLOCK;
                fcntl(connfd, F_SETFL, fl);
                while(1) {
                    recv_msg(connfd);
                            
                    printf("Server : ");  
                    memset(buff, 0, sizeof(buff));
                    scanf("%s", buff);
                    if (strncmp("exit", buff, 4) == 0) { 
                        printf("Server exit...\n");
                        send_msg(connfd, buff);
                        shutdown(connfd, SHUT_RDWR);
                        close(connfd);
                        goto LOOP;
                    } else
                    send_msg(connfd, buff);
                }
            } 
        }       
}