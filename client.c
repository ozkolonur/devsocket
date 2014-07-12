#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 4656
#define MAXDATASIZE 100

int main(int argc, char *argv[])
{
    int sockfd;
    int numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in client_addr;
    
    printf("arg=%s\n",argv[1]);
    if (argc != 2)
    {
	printf("Usage: client hostname");
	exit(1);
    }

    if ((he=gethostbyname(argv[1])) == NULL) 
    {  
        herror("client gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
	perror("client socket");
	exit(1);
    }
    client_addr.sin_family = PF_INET;    // host byte order 
    client_addr.sin_port = htons(SERVER_PORT);  // short, network byte order 
    client_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(client_addr.sin_zero, '\0', sizeof client_addr.sin_zero);

    if (connect(sockfd, (struct sockaddr *)&client_addr, sizeof client_addr) == -1) 
    {
	perror("connect");
        exit(1);
    }

    if ((numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
    {
	perror("recv");
	exit(1);
    }

    buf[numbytes] = '\0';
    printf("Received: %s",buf);
    close(sockfd);
    return 0;

}


