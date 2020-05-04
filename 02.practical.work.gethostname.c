#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

extern int h_errno;

int main() {

	char hostname[256];
	printf("* Enter host name: ");
    	scanf("%s", hostname);

    struct hostent *host_ip_data = gethostbyname(hostname); 
	char *host_ip = inet_ntoa(*(struct in_addr*) host_ip_data->h_addr_list[0]);

	printf("* The ip of %s is: %s\n", hostname, host_ip);

}
