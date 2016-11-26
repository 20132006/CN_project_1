#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

#define CLIENTMAX 1000
#define BYTES 1024

char *ROOT;
int listenfd, clients[CLIENTMAX];
void error(char *);
void startServer(char *);
void respond(int);

int main(int argc, char* argv[])
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;

    char PORT[6];
    ROOT = getenv("PWD");

  	if(argc != 2)
  		strcpy(PORT,"10000");
  	else
		  strcpy(PORT,argv[1]);

    int slot=0;

    printf("Server started at port no. %s%s%s with root directory as %s%s%s\n","\033[92m",PORT,"\033[0m","\033[92m",ROOT,"\033[0m");

    int i;
    for (i=0; i<CLIENTMAX; i++)
        clients[i]=-1;
    startServer(PORT);

    while (1)
    {
      ServerSocket new_sock;
	    server.accept ( new_sock );
  		//Write your own code 1.
  		//When a client sends request then catch the request(using accept function in "sys/socket.h") and do respond (using respond function in this code).
  		//Check the socket number and if there is some error then handle the error.
  		//You must make a code which can be handle to multiple users (I think that you can implement this using 'fork()' but it is not essentially required.)
    }

    return 0;
}

void startServer(char *port)
{
    struct addrinfo hints, *res, *p;

    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0)
    {
        perror ("getaddrinfo() error");
        exit(1);
    }

    for (p = res; p!=NULL; p=p->ai_next)
    {
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}

void respond(int n)
{
    char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
    int rcvd, fd, bytes_read;

    memset( (void*)mesg, (int)'\0', 99999 );

    rcvd=recv(clients[n], mesg, 99999, 0);

    if (rcvd<0)
        fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else
    {
        reqline[0] = strtok (mesg, " \t\n");
        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(clients[n], "HTTP/1.0 400 Bad Request\n", 25);
            }
            else
            {
                if ( strncmp(reqline[1], "/\0", 2)==0 )
                    reqline[1] = "/index.html";

        				//Write your own code 2.
        				//make path variable which is root-path (ROOT) + /index.html (in reqline[1])
        				//open file in the path to read-only. If it has some error then show error statements to current client.
        				//If the file is opened successfully then a server sends index.html file to current client.
            }
        }
    }

    shutdown (clients[n], SHUT_RDWR);
    close(clients[n]);
    clients[n]=-1;
}
