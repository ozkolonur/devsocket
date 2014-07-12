#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define SERVER_PORT 4656
#define MAXCLIENTS 100
#define BUFSIZE	100

void *fserveClient(void *data)
{
    char cbuf[BUFSIZE];
    int  isockn = (int)data, i = 0;

    sprintf(cbuf, "Hello Client %d\n", isockn);
    if((i = write(isockn, cbuf, BUFSIZE)) < 0)
	perror("writing to socket");
    return;
};

int main(int argc, char *argv[])
{
    int isockfd=0, isockn=0, inthreads=0;
    int inewfd=0;
    struct sockaddr_in stServerAddr;
    struct sockaddr_in stClientAddr;
    pthread_t threadID[MAXCLIENTS];
    int true = 1;
    int x = 0;

    printf("Server Started\n");
    if ((isockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
	perror("Server socket create");
	exit(1);
    }
    
    if (setsockopt(isockfd, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int)) == -1)
    {
	perror("Server socket option");
	exit(1);
    }
    
    stServerAddr.sin_family = PF_INET;
    stServerAddr.sin_port = htons(SERVER_PORT);
    stServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(stServerAddr.sin_zero, '\0', sizeof(stServerAddr.sin_zero));

    if (bind(isockfd, (struct sockaddr *)&stServerAddr, sizeof(stServerAddr)) == -1)
    {
	perror("Server socket bind");
	exit(1);
    }
    
    if (listen(isockfd, MAXCLIENTS) == -1)
    {
	perror("Server socket listen");
	exit(1);
    }

    while(1){
	//accept a connection
        if( (isockn = accept(isockfd, NULL, NULL)) < 0)
            perror("accept");
                
        //spawn a thread for each client
        if(pthread_create(&threadID[x++], NULL, (void *)&fserveClient, (int)isockn) != 0)
            perror("pthread");
                
        if(inthreads++ >= MAXCLIENTS)
            break;
    }
        
    //wait for all threads
    for(x = 0; x < inthreads; x++)
        if(pthread_join(threadID[x], NULL) < 0)
            perror("pthread_join");
        
    //cleanup
    close(isockfd);
        
    return 0;
}



