/* tcp-client.c */

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int get_stdin( char * buffer, int maxlen )
{
  int n, i = 0;

  while ( ( n = read( STDIN_FILENO, buffer + i, maxlen - i ) ) > 0 )
  {
    i += n;
    if ( i == maxlen ) break;
  }

  if ( n != 0 )
  {
    perror( "Error reading stdin" );
    exit( EXIT_FAILURE );
  }

  /* return the number of bytes read including the last read */
  return i;
}

int main() {
    /* create TCP client socket (endpoint) */
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    
    if (sd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }
    
#if 0    
    /* localhost maps to 127.0.0.1, which stays on the local machine */
    struct hostent * hp = gethostbyname("localhost");
    
    struct hostent * hp = gethostbyname("128.113.126.29");
#endif
    
                                        /* "localhost" -- the local machine */
                                        /* "128.113.7.12" -- this might work */
    struct hostent * hp = gethostbyname("localhost");
    
    if (hp == NULL) {
        fprintf(stderr, "ERROR: gethostbyname() failed\n");
        return EXIT_FAILURE;
    }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    memcpy((void *)&server.sin_addr, (void*)hp->h_addr, hp->h_length);
    unsigned short port = 12345;
    server.sin_port = htons(port);
    
    printf("server address is %s\n", inet_ntoa(server.sin_addr));
    
    printf("connection to server...\n");
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connect() failed");
        return EXIT_FAILURE;
    }
    
    /* The implementation of the application layer is below */
#if 0
    char * msg = "Hello world";
    int n = write(sd, msg, strlen(msg));
    
    if (n < strlen(msg)) {
        perror("write() failed");
        return EXIT_FAILURE;
    }
#endif

#if 1
    int length;
    char temp[BUFFER_SIZE];
    length = get_stdin( temp, BUFFER_SIZE );
    int n = write(sd, temp, length);
    
    if (n < length) {
        perror("write() failed");
        return EXIT_FAILURE;
    }
#endif
    
    char buffer[BUFFER_SIZE];
    n = read(sd, buffer, BUFFER_SIZE-1); /* BLOCKING */
    if ( n == -1) {
        perror("read() failed");
        return EXIT_FAILURE;
    } else if (n == 0) { /* socket was closed */
        printf("Rcvd no data; also, server socket was closed\n");
    } else {
        buffer[n] = '\0'; /* assume we rcvd text-based data */
        printf("Rcvd from server: %s\n", buffer);
    }
    
    close(sd);
    return EXIT_SUCCESS;
}