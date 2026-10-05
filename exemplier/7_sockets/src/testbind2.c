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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

/**
 * @file testbind2.c
 * @brief Demonstrates creating and binding a TCP/IP stream socket to a
 * specific host.
 *
 * The host name (or address) given on the command line is resolved with
 * getaddrinfo() (gethostbyname() was removed from POSIX in 2008). A stream
 * socket is then created and bound, on port 5001, to the first address that
 * works (slide « getaddrinfo() – essayer chaque adresse »).
 *
 * \code{.bash}
 *   $ ./testbind2 localhost
 *   Socket bound to localhost, port 5001
 * \endcode
 */

#define PORT "5001"

int main(int argc, char *argv[]) {
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;
    int sd = -1;
    int err;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;     /* IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    err = getaddrinfo(argv[1], PORT, &hints, &res);
    if (err != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    /* Try each address until the socket can be bound */
    for (p = res; p != NULL; p = p->ai_next) {
        sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sd == -1) {
            continue;
        }
        if (bind(sd, p->ai_addr, p->ai_addrlen) == 0) {
            break;                   /* success */
        }
        close(sd);
        sd = -1;
    }
    freeaddrinfo(res);

    if (sd == -1) {
        fprintf(stderr, "Unable to bind a socket to %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Here you could call listen() if needed */

    printf("Socket bound to %s, port %s\n", argv[1], PORT);
    close(sd);

    return EXIT_SUCCESS;
}
