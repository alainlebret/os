/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h> /* Internet structures and functions. */
#include <sys/socket.h> /* Socket functions. */

/**
 * @file rotn_server.c
 * @brief A TCP server that performs ROTn obfuscation on received text.
 *
 * This server listens for TCP connections on port 6789. For each incoming
 * connection, it forks a new process to handle the connection. It reads data
 * from the connection, applies ROTn obfuscation, and sends the result back.
 * The server also handles SIGCHLD signals to prevent zombie processes.
 *
 * @author Alain Lebret
 * @version 1.0
 * @date 2020-09-12
 */

#define FOREVER for (;;)
#define MAX_LINE 16384

/**
 * @brief Apply ROTn obfuscation to a character.
 * 
 * @param c Character to obfuscate.
 * @param rot Number of positions to rotate.
 * @return char The obfuscated character.
 */
char rot_char(char c, int rot)
{
    char result;

    result = c;
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M')) {
        result += rot;
    } else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z')) {
        result -= rot;
    }

    return result;
}

/**
 * @brief Signal handler for SIGCHLD.
 *
 * Cleans up zombie processes created by finished child processes.
 *
 * @param sig Signal number (not used).
 */
void sigchld_handler(int sig) {
    /* Wait for all dead processes. */
    /* We use a non-blocking call to avoid hanging if a child hasn't exited yet. */
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

/**
 * @brief Handles a single client connection.
 * 
 * Reads data from the client, applies ROTn obfuscation, and sends it back.
 * Closes the socket once the communication is done.
 *
 * @param fd File descriptor for the client socket.
 */
void handle_child(int fd) {
    char outbuf[MAX_LINE + 1];
    size_t outbuf_used;
    ssize_t result;

    outbuf_used = 0;

    FOREVER {
        char ch;

        /* Read each character from the client */
        result = recv(fd, &ch, 1, 0);
        if (result == 0) {
            break;
        } else if (result == -1) {
            perror("read");
            break;
        }

        /* Test not to overflowing the output buffer. */
        if (outbuf_used < sizeof(outbuf)) {
            outbuf[outbuf_used++] = rot_char(ch, 3);
        }

        if (ch == '\n') {
            /* Send message to the socket of the incoming connection */
            send(fd, outbuf, outbuf_used, 0);
            outbuf_used = 0;
        }
    }

    close(fd);  /* Close the client socket */
}

/**
 * @brief Sets up and runs the ROTn server.
 *
 * Initializes the server socket, listens for incoming connections,
 * and handles them by forking new processes.
 */
void run() {
    int listener;
    struct sockaddr_in sin;
    struct sigaction sa;

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    sin.sin_family = AF_INET;
    /* Set IP address to localhost */
    sin.sin_addr.s_addr = 0;
    /* Set port number, using htons function to use proper byte order */
    sin.sin_port = htons(6789);

   /* 
	* Create the socket. The three arguments are: 1) Internet domain 2) Stream
    * socket 3) Default protocol (TCP in this case)
    */
    listener = socket(AF_INET, SOCK_STREAM, 0);

    /* Bind the address struct to the socket */
    if (bind(listener, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }

    /* Listen on the socket, with 10 max connection requests queued */
    if (listen(listener, 10) < 0) {
        perror("listen");
        return;
    }

    /* Set up SIGCHLD handler to prevent zombie processes */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &sigchld_handler;
    sigaction(SIGCHLD, &sa, NULL);

    FOREVER {
        struct sockaddr_storage ss;
        socklen_t slen;
		int fd;
		
        slen = sizeof(ss);
        /* Accept call creates a new socket for the incoming connection */
        fd = accept(listener, (struct sockaddr *) &ss, &slen);
        if (fd < 0) {
            perror("accept");
        } else {
            if (fork() == 0) {
                handle_child(fd);
                exit(EXIT_SUCCESS);
            } else {
                close(fd);  /* Parent doesn't need this socket */
            }
        }
    }
}

int main(void) {
    run();
    return EXIT_SUCCESS;
}
