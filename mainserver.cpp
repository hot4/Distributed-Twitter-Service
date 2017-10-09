#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
	int sd = socket( PF_INET, SOCK_STREAM, 0 );
    /* here, the sd is a socket descriptor (part of the fd table) */

    if ( sd == -1 )
    {
    	perror( "socket() failed" );
    	return EXIT_FAILURE;
  	}
  	struct sockaddr_in server;
  	server.sin_family = PF_INET;
  	server.sin_addr.s_addr = INADDR_ANY;  /* allow any IP address to connect */
  	unsigned short port = 8123;
  	server.sin_port = htons( port );
  	int len = sizeof( server );

  	if ( bind( sd, (struct sockaddr *)&server, len ) == -1 )
  	{
    	perror( "bind() failed" );
    	return EXIT_FAILURE;
  	}

  	/* identify this port as a listener port */
  	if ( listen( sd, 5 ) == -1 ) // listens for up to 5 clients
  	{
    	perror( "listen() failed" );
    	return EXIT_FAILURE;
  	}
  	struct sockaddr_in client;
  	int fromlen = sizeof( client );
  	int n;
  	char buffer[ BUFFER_SIZE ];
  	int numusers=5;
  	pthread_t tid[ numusers ];
  	int i10=0;
  	while ( 1 )
  	{
    	printf( "SERVER: Blocked on accept()\n" );
    	int newsd = accept( sd, (struct sockaddr *)&client, (socklen_t *)&fromlen );
    	rc = pthread_create( &tid[i10], NULL, whattodo1, &newsd );
        if ( rc != 0 )
        {
            fprintf( stderr, "MAIN: Could not create child thread (%d)\n", rc );
        }
        //unsigned int * x;
        //pthread_join( tid4[i10], (void **)&x );
    	printf( "SERVER: Accepted new client connection on sd %d\n", newsd );
    	i10++;

  	}



	return 0;
}

void * whattodo1( void * arg)
{
	int newsd= *(int*) arg;
	do
    	{

      		printf( "SERVER: Blocked on recv()\n" );

      		/* recv() will block until we receive data (n > 0)
          or there's an error (n == -1)
           or the client closed the socket (n == 0) */
      		n = recv( newsd, buffer, BUFFER_SIZE, 0 );

      		if ( n == -1 )
      		{
        		perror( "recv() failed" );
        		return EXIT_FAILURE;
      		}
      		else if ( n == 0 )
      		{
        		printf( "SERVER: Rcvd 0 from recv(); closing socket...\n" );
      		}
      		else /* n > 0 */
      		{
        		buffer[n] = '\0';    /* assume this is text data */
        		printf( "SERVER: Rcvd message from %s: %s\n", inet_ntoa( (struct in_addr)client.sin_addr ), buffer );

        		/* send ACK message back to the client */
        		n = send( newsd, "ACK\n", 4, 0 );

        		if ( n != 4 )
        		{
          			perror( "send() failed" );
          			return EXIT_FAILURE;
        		}
      		}
    	}
    	while ( n > 0 );

    	close( newsd );
}
