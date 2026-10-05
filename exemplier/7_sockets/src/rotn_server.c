/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <netinet/in.h> /* Internet structures and functions. */
#include <arpa/inet.h>  /* htons(), htonl() */
#include <sys/socket.h> /* Socket functions. */
#include <sys/wait.h>

/**
 * @file rotn_server.c
 * @brief A TCP server that performs ROTn obfuscation on received text.
 *
 * This server listens for TCP connections on port 5001. For each incoming
 * connection, it forks a new process to handle the connection. It reads data
 * from the connection, applies ROTn obfuscation, and sends the result back.
 * The server also handles SIGCHLD signals to prevent zombie processes.
 */

#define MAX_LINE 16384
#ifndef PORT
#define PORT 5001
#endif

static int send_all(int fd, const char *buffer, size_t len) {
    size_t total_sent = 0;

    while (total_sent < len) {
        ssize_t sent = send(fd, buffer + total_sent, len - total_sent, 0);
        if (sent < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (sent == 0) {
            return -1;
        }
        total_sent += (size_t) sent;
    }

    return 0;
}

/**
 * @brief Apply ROTn obfuscation to a character.
 * 
 * @param c Character to obfuscate.
 * @param rot Number of positions to rotate.
 * @return char The obfuscated character.
 */
char rot_char(char c, int rot) {
    char result;

    /* rotation modulo 26 (the former "+rot for a-m, -rot for n-z" was
     * only correct for rot = 13) */
    result = c;
    if (c >= 'a' && c <= 'z') {
        result = (char) ('a' + (c - 'a' + rot) % 26);
    } else if (c >= 'A' && c <= 'Z') {
        result = (char) ('A' + (c - 'A' + rot) % 26);
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
    int saved_errno = errno;   /* waitpid() modifies errno */
    (void) sig;
    /* Wait for all dead processes. */
    /* We use a non-blocking call to avoid hanging if a child hasn't exited yet. */
    while (waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
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

    while (1) {
        char ch;

        /* Read each character from the client */
        result = recv(fd, &ch, 1, 0);
        if (result == 0) {
            break;
        } else if (result == -1) {
            perror("recv");
            break;
        }

        outbuf[outbuf_used++] = rot_char(ch, 3);

        /* Send at the end of a line, or when the buffer is full (very long
           line): no character is lost, the '\n' included */
        if (ch == '\n' || outbuf_used == sizeof(outbuf)) {
            /* Send message to the socket of the incoming connection */
            if (send_all(fd, outbuf, outbuf_used) == -1) {
                perror("send");
                break;
            }
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
void run(void) {
    int listener;
    int yes = 1;
    struct sockaddr_in sin;
    struct sigaction sa;

    /* SECURITY NOTE: this demo server has no authentication, encryption, or rate limiting.
     * Production services need TLS, access control, timeouts, and DoS protections. */
    /*---- Configure settings of the server address struct ----*/
    memset(&sin, 0, sizeof(sin));
    /* Address family = Internet */
    sin.sin_family = AF_INET;
    /* Any local address (all the interfaces) */
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    /* Set port number, using htons function to use proper byte order */
    sin.sin_port = htons(PORT);

    /*
     * Create the socket. The three arguments are: 1) Internet domain 2) Stream
     * socket 3) Default protocol (TCP in this case)
     */
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /* Allow restarting the server despite connections in TIME_WAIT */
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* Bind the address struct to the socket */
    if (bind(listener, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* Listen on the socket, with 10 max connection requests queued */
    if (listen(listener, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Set up SIGCHLD handler to prevent zombie processes */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    signal(SIGPIPE, SIG_IGN);  /* client left: send() fails with EPIPE */

    while (1) {
        struct sockaddr_storage ss;
        socklen_t slen;
        int fd;

        slen = sizeof(ss);
        /* Accept call creates a new socket for the incoming connection */
        fd = accept(listener, (struct sockaddr *) &ss, &slen);
        if (fd < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("accept");
        } else {
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
            } else if (pid == 0) {
                close(listener);  /* The child does not accept connections */
                handle_child(fd);
                exit(EXIT_SUCCESS);
            }
            close(fd);  /* Parent doesn't need this socket */
        }
    }
}

int main(void) {
    run();

    return EXIT_SUCCESS;
}
