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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @file resolve_name.c
 * @brief Demonstrates resolving a hostname to its IP address(es) using
 * getaddrinfo().
 *
 * This program takes a hostname as a command line argument and resolves it
 * to its corresponding IP address(es). It replaces the historical
 * gethostbyname() + inet_ntoa() version: gethostbyname() only handles IPv4,
 * is not reentrant and was removed from POSIX in 2008; getaddrinfo() and
 * inet_ntop() handle IPv4 and IPv6.
 *
 * \code{.bash}
 *   $ ./resolve_name localhost
 *   localhost addresses: ::1 127.0.0.1
 * \endcode
 */

/**
 * @brief Resolves the given hostname and prints its IP address(es).
 * 
 * @param hostname The hostname to resolve.
 */
void resolve_and_print(const char *hostname) {
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;
    char ipstr[INET6_ADDRSTRLEN];
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; /* AF_INET or AF_INET6 to force version */
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    printf("%s addresses: ", hostname);
    for (p = res; p != NULL; p = p->ai_next) {
        void *address = NULL;
        if (p->ai_family == AF_INET) { /* IPv4 */
            struct sockaddr_in *ipv4 = (struct sockaddr_in *) p->ai_addr;
            address = &(ipv4->sin_addr);
        } else { /* IPv6 */
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *) p->ai_addr;
            address = &(ipv6->sin6_addr);
        }

        /* Convert the IP to a string and print it: */
        inet_ntop(p->ai_family, address, ipstr, sizeof ipstr);
        printf("%s ", ipstr);
    }
    printf("\n");

    freeaddrinfo(res); /* Free the linked list */
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    resolve_and_print(argv[1]);

    return EXIT_SUCCESS;
}
