#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h> /* close file descriptor */
#include <poll.h> /* struct poll */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

/* Global directory name */
std::string DIRECTORY = "./storage/";

/**
 * @effects creates directory labeled "storage" if none exists
 */
void createDirectory() {
	struct stat st = {0};
	/* Check if director already exists */
  	if (stat(DIRECTORY.c_str(), &st) == -1) {
  		mkdir(DIRECTORY.c_str(), 0700);
  	}
}

int main(int argc, char* argv[])
{
	int rc, on, nfds = 1;
	// int currSize = 0;
	int sdTCP = -1;

    printf("Started server\n");
    fflush(stdout);
    
    /* Create the socket (endpoint) to receive connections on */
    sdTCP = socket(AF_INET, SOCK_STREAM, 0);
    if (sdTCP == -1) {
        perror("ERROR: socket() failed");
        return EXIT_FAILURE;
    }

    /* Make socket reusable */
    rc = setsockopt(sdTCP, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    if (rc == -1) {
    	perror("ERROR: setsockopt() failed");
    	close(sdTCP);
    	return EXIT_FAILURE;
    }

    /* Bind TCP socket */
    struct sockaddr_in serverTCP;
    memset(&serverTCP, 0, sizeof(serverTCP));
    serverTCP.sin_family = AF_INET;
    serverTCP.sin_addr.s_addr = htonl(INADDR_ANY);
    serverTCP.sin_port = htons(SERVER_PORT);
    rc = bind (sdTCP, (struct sockaddr*) &serverTCP, sizeof(serverTCP));
    if (rc == -1) {
    	perror("ERROR: bind() failed");
    	close(sdTCP);
    	exit(EXIT_FAILURE);
    }

    /* Set the listen back log */
    rc = listen(sdTCP, 32);
    if (rc == -1) {
    	perror("ERROR: listen() failed");
    	close(sdTCP);
    	exit(EXIT_FAILURE);
    }

    /* Initialize Server Poll FD */
    struct pollfd fds[4];
    memset(fds, 0, sizeof(fds));

    /* Set up initial listening socket */
    fds[0].fd = sdTCP;
    fds[0].events = POLLIN;

    /* Initialize Console Poll FD */
    struct pollfd cfd = { STDIN_FILENO, POLLIN, 0 };

    /* Timeout (based on milliseconds). If no activity after 3 seconds, 
       query command line to see if there are any command arguments */
    int timeout = 3*1000;

    createDirectory();

    while (1) {
    	/* Block for 3 seconds to see if there are any incoming messages */
    	printf("Waiting on recveiving messages...\n");
    	rc = poll(fds, nfds, timeout);

    	if (rc == -1) {
    		/* ERROR */
    		perror("ERROR: server poll() failed");
    		break;
    	} else if (rc == 0) {
    		/* Block for 3 seconds to see if there are inputs from the console */
    		printf("Waiting on input from console...\n");
    		rc = poll(&cfd, nfds, 5*1000); 

    		if (rc == -1) {
    			/* ERROR */
    			perror("EEROR: console poll() failed");
    			break;
    		} else if (rc == 0) {
    			/* Loop back to server poll() since no console commands were provided */
    			continue;
    	    } else {
	    		/* Get command from console */
			    std::string cmd;
			    getline(std::cin, cmd, '\n');
			    
			    /* Basic print to terminal to verify proper reading from console */
			    std::cout << cmd << std::endl;
			    fflush(stdout);

			    /* Checks which command was prompted */
			    if (cmd == "Tweet") {
			    	printf("Tweet Success\n");
			    	fflush(stdout);
			    } else if (cmd == "Block") {
			    	printf("Block Success\n");
			    	fflush(stdout);
			    } else if (cmd == "Unblock") {
			    	printf("Unblock Success\n");
			    	fflush(stdout);
			    } else {
			    	printf("Incorrect command provided. Use {Tweet, Block, Unblock}\n");
			    	fflush(stdout);
			    }
			}
    	}
    }

    /* Close file descriptors */
    for (int i = 0; i < nfds; i++) {
    	if (fds[i].fd >= 0) {
    		close(fds[i].fd);
    	}
    }
    
    return EXIT_SUCCESS;
}