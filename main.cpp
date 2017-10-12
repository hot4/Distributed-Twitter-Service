#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h> /* close file descriptor */
#include <poll.h> /* struct poll */
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h> /* gethostbyname */
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

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

/**
  * @effects Sends input from console to specified port
  * @returns true if no library functions failed, false otherwise
  */
bool tweet() {
    /* Create TCP client socket (endpoint) */
    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket() failed");
        return false;
    }

    /* Get localhost information */
    struct hostent * hp = gethostbyname("localhost");
    if (hp == NULL) {
        fprintf(stderr, "ERROR: gethostbyname() failed\n");
        return false;
    }

    /* Establish connection to server at specified port */
    std::string outPort;
    printf("Input Port to Send Message: ");
    fflush(stdout);
    getline(std::cin, outPort, '\n');

    struct sockaddr_in outGoing;
    outGoing.sin_family = AF_INET;
    memcpy((void *)&outGoing.sin_addr, (void*)hp->h_addr, hp->h_length);
    outGoing.sin_port = htons(atoi(outPort.c_str()));
    if (connect(sd, (struct sockaddr *)&outGoing, sizeof(outGoing)) == -1) {
        perror("connect() failed");
        return false;
    }

    /* Get message to be sent to outgoing port */
    std::string message;
    printf("Input Tweet Message: ");
    fflush(stdout);
    getline(std::cin, message, '\n');

    /* Send */
    char messageArr[BUFFER_SIZE];
    strcpy(messageArr, message.c_str());
    int n = write(sd, messageArr, sizeof(messageArr));
    int length = sizeof(messageArr);
    if (n < length) {
        perror("write() failed");
        return false;
    }
    close(sd);
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("ERROR: Invalid amount of command line arguments. Include port number.\n");
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }

    int rc, on, nfds = 1;
    int currSize = 0;
    int port = atoi(argv[1]), sdTCP = -1, newSD = -1;
    bool endServer = false, closeConn = false, compressArr = false;

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
    serverTCP.sin_port = htons(port);
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

    do {
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
            rc = poll(&cfd, nfds, timeout); 

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

                /* Checks which command was prompted */
                if (cmd == "Tweet") {
                    if (!tweet()) return EXIT_FAILURE;
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
        } /* End of poll errors or command console */

        /* Descriptor(s) are readable. Filter through all */
        currSize = nfds;
        /* Iterate through FDs to find those with a return value of POLLIN */
        /* Determine if descriptor is listening or active connection */
        for (int i = 0; i < currSize; i++) {
            if(fds[i].revents == 0) continue;

            /* Check if unexpected result */
            if(fds[i].revents != POLLIN) {
                perror("ERROR: Unexpected revents result");
                endServer = true;
                break;
            }

            /* Check if incoming connection is readable */
            if (fds[i].fd == sdTCP) {
                printf("TCP listen socket is readable\n");

                /* Accept incoming connection before looping back to listen for more incoming connections or console commands */
                newSD = accept(sdTCP, NULL, NULL);
                if (newSD == -1) {
                    perror("ERROR: accept() failed");
                    endServer = true;
                    break;
                }

                /* Add incoming connection to poll fd struct */
                printf("New incoming connection - %d\n", newSD);
                fflush(stdout);
                fds[nfds].fd = newSD;
                fds[nfds].events = POLLIN;
                nfds++;
            } else {
                /* Not a listening socket, therefore, existing connection must be readable */
                printf("Descriptor %d is readable\n", fds[i].fd);
                closeConn = false;
                /* Retrieve incoming data on current socket before continuing */
                do {
                    char recvBuffer[BUFFER_SIZE];
                    rc = recv(fds[i].fd, recvBuffer, sizeof(recvBuffer), 0);
                    if (rc == -1) {
                        perror("ERROR: recv() failed");
                        closeConn = true;
                        break;
                    } else if (rc == 0) {
                        /* Check if client closed connection */
                        printf("Connection closed\n");
                        closeConn = true;
                        break;
                    } else {
                        /* BASIC Output to verify correct message was received */
                        printf("Tweet Message: %s\n", recvBuffer);   
                    }
                } while (true);

                /* Clean up this active connection and clean up process */
                /* Remove descriptor */
                if (closeConn) {
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    compressArr = true;
                } 
            } /* End of existing connection is readable */
        } /* End of loop over pollable descriptors */

        /* Remove current connection from fds */
        if (compressArr) {
            compressArr = false;
            /* Iterate through fds and find the closed connection */
            for (int i = 0; i < nfds; i++) {
                if (fds[i].fd == -1) {
                    /* Iterate through remaining fds and replace current closed connection */
                    for (int j = i; j < nfds; j++) {
                        /* Only shift fd information because all events are POLLIN and revents is output */
                        fds[j].fd = fds[j+1].fd;
                    }
                    nfds--;
                }
            }
        }

    } while (!endServer);
    
    /* Close file descriptors */
    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd >= 0) {
            close(fds[i].fd);
        }
    }

    return EXIT_SUCCESS;
}