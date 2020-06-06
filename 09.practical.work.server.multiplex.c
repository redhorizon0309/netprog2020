#include <fcntl.h> 
#include <unistd.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/uio.h>

#define MAX 80 
#define MAX_CLIENT 20
#define PORT 8784 
#define SA struct sockaddr 

char buff[MAX];

int send_msg(int sockfd,char buff[MAX]) {
    int n = strlen(buff) + 1;
    send(sockfd, buff, n, 0);
    if (strncmp("exit", buff, 4) == 0) {
        printf("Disconnecting ...");
        return 1;
    }
    return 0;
}
  
int recv_msg(int sockfd) {
    char buff[MAX];
    memset(buff, 0, sizeof(buff));
    int n = recv(sockfd, buff, sizeof(buff), 0);
    if (n < 0) {
        return -1;
    } else {
        printf("Client: %s\n", buff);
        if (strncmp("exit", buff, 4) == 0) {
            send(sockfd, buff, 5, 0);
            close(sockfd);
            return 1;
        }   
        return 0;
    } 
}

  
// Driver function 
int main() { 
    int sockfd, len; 
    struct sockaddr_in servaddr, cli; 
	int connfd[MAX_CLIENT] = {0};
	char s[100]; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
     
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    int fl = fcntl(sockfd, F_GETFL, 0);
    fl |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, fl);

    bzero(&servaddr, sizeof(servaddr));
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
    

    if (listen(sockfd, 5) < 0) { 
        printf("Error listening\n");
    }
	
	while (1) {			// for accepting new clients
		fd_set set;		// prepare the set
		FD_ZERO(&set);
		FD_SET(sockfd, &set);		// add the listening socket to the set
		FD_SET(STDIN_FILENO, &set);	// add the STDIN to the set
		int maxfd = sockfd;
		for (int i = 0; i < MAX_CLIENT; i++) {
			// add connected client sockets to set
			if (connfd[i] > 0) {
				FD_SET(connfd[i], &set);
				if (connfd[i] > maxfd) maxfd = connfd[i];
			}
		}
		// poll and wait, blocked indefinitely
		select(maxfd+1, &set, NULL, NULL, NULL);

		// from stdin?
		if (FD_ISSET(STDIN_FILENO, &set)) {
			memset(s, 0, sizeof(s));
			int n = read(STDIN_FILENO, s, sizeof(s));
			// send everything to all connected clients
			for (int i = 0; i < MAX_CLIENT; i++) { 
				if (connfd[i] > 0) {
					write(connfd[i], s, strlen(s));
					break;
				}
			}			
		}

		if (FD_ISSET(sockfd, &set)) {

            len = sizeof(cli); 
			int clientfd = accept(sockfd, (SA*)&cli, &len); 
			// make it nonblocking
			fl = fcntl(clientfd, F_GETFL, 0);
			fl |= O_NONBLOCK;
			fcntl(clientfd, F_SETFL, fl);

			// add it to the clientfds array
			for (int i = 0; i < MAX_CLIENT; i++) { 
				if (connfd[i] == 0) {
					connfd[i] = clientfd;
					printf("accepted client %d, index #%d\n", clientfd, i);
					break; 
				}
			}
		}

		// is that data from a previously-connect client?
		for (int i = 0; i < MAX_CLIENT; i++) {
			if (connfd[i] > 0 && FD_ISSET(connfd[i], &set)) {
				if (read(connfd[i], s, sizeof(s)) > 0) {
					printf("client %d says: %s\n", connfd[i], s);
				}
				else {
					// some error. remove it from the "active" fd array
					printf("client %d has disconnected.\n", connfd[i]);
					connfd[i] = 0;
				}
			} 
		}
	}

    return 0;
}