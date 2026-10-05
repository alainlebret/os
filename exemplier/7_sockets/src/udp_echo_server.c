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
#include <stdio.h>      /* printf(), perror() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* memset() */
#include <unistd.h>     /* close() */
#include <arpa/inet.h>  /* htons(), htonl(), inet_ntop() */
#include <netinet/in.h> /* struct sockaddr_in, INADDR_ANY */
#include <sys/socket.h> /* socket(), bind(), recvfrom(), sendto() */

/**
 * @file udp_echo_server.c
 * @brief Running example: UDP echo receiver on port 5001.
 *
 * Lecture: chapter os-11 « Sockets » ("Fil rouge – récepteur UDP (1/2)"
 * and "(2/2)").
 *
 * The receiver binds its socket to port 5001, waits for datagrams with
 * recvfrom() and sends each one back to its sender with sendto().
 *
 * Key points:
 *   - only the type changes: SOCK_DGRAM; no listen(), no accept();
 *   - a single socket for all the clients;
 *   - the echo goes back to the address filled in by recvfrom();
 *   - one reception = one whole datagram (a too small buffer truncates it).
 * The port can be changed at compile time: -DPORT=5002.
 *
 * \code{.bash}
 *   $ ./udp_echo_server                 # terminal 1
 *   5 octets de 127.0.0.1
 *   $ ./udp_echo_client 127.0.0.1 salut # terminal 2
 *   écho : salut
 * \endcode
 */

#ifndef PORT
#define PORT 5001
#endif

/**
 * @brief Displays the error message (perror), then exits.
 */
void erreur(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    struct sockaddr_in adr, cli;
    char buf[1500];

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        erreur("socket");
    }
    memset(&adr, 0, sizeof adr);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(PORT);
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (struct sockaddr *) &adr, sizeof adr) == -1) {
        erreur("bind");
    }

    for (;;) {
        socklen_t lg = sizeof cli;
        char ip[INET_ADDRSTRLEN];
        ssize_t n = recvfrom(fd, buf, sizeof buf, 0,
                             (struct sockaddr *) &cli, &lg);
        if (n == -1) {
            erreur("recvfrom");
        }
        inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof ip);
        printf("%zd octets de %s\n", n, ip);
        fflush(stdout);
        if (sendto(fd, buf, n, 0,               /* echo */
                   (struct sockaddr *) &cli, lg) == -1) {
            perror("sendto");
        }
    }
}
