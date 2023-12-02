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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @file testbind1.c
 * @brief Demonstrates creating and binding a TCP/IP stream socket.
 *
 * This program creates a TCP/IP stream socket and binds it to a specified
 * port number on the local host. The program is a basic demonstration of
 * socket programming in C.
 *
 * @author Alain Lebret
 * @version 1.0
 * @date 2012-04-10
 */

int main(int argc, char *argv[]) {
    int sd;
    int port = 5432;  /* Default port */
    struct sockaddr_in name;

    if (argc > 1) {
        port = atoi(argv[1]);  /* Allow port to be set via command line */
    }

    /* Create the socket */
    sd = socket(PF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    /* Give a name to the socket */
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *) &name, sizeof(name)) < 0) {
        perror("bind() failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    /* Here you could call to listen() if needed */
 
    printf("Socket bound to port %d\n", port);
    close(sd);

    return EXIT_SUCCESS;
}
