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
#include <stdio.h>      /* printf(), fprintf(), perror() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* memset(), strlen() */
#include <unistd.h>     /* close() */
#include <arpa/inet.h>  /* htons(), inet_pton() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <sys/socket.h> /* socket(), sendto(), recvfrom() */

/**
 * @file udp_echo_client.c
 * @brief Running example: UDP echo sender (port 5001).
 *
 * Lecture: chapter os-11 « Sockets » ("Fil rouge – émetteur UDP (1/2)"
 * and "(2/2)").
 *
 * The sender sends its second argument in one datagram to the receiver
 * (udp_echo_server), then waits for the echo and displays it.
 *
 * Key points:
 *   - the sender does not need bind(): an ephemeral port is chosen at the
 *     first sendto();
 *   - if the datagram or the echo is lost, recvfrom() waits forever: a
 *     timeout should be set (SO_RCVTIMEO, or poll()).
 * The port can be changed at compile time: -DPORT=5002.
 *
 * \code{.bash}
 *   $ ./udp_echo_client 127.0.0.1 salut
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

int main(int argc, char *argv[]) {
    struct sockaddr_in srv;
    char buf[1500];

    if (argc != 3) {
        fprintf(stderr, "usage : %s ip msg\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        erreur("socket");
    }
    memset(&srv, 0, sizeof srv);
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &srv.sin_addr) != 1) {
        fprintf(stderr, "adresse invalide\n");
        exit(EXIT_FAILURE);
    }

    if (sendto(fd, argv[2], strlen(argv[2]), 0,
               (struct sockaddr *) &srv, sizeof srv) == -1) {
        erreur("sendto");
    }
    ssize_t n = recvfrom(fd, buf, sizeof buf - 1, 0, NULL, NULL);
    if (n == -1) {
        erreur("recvfrom");
    }
    buf[n] = '\0';
    printf("écho : %s\n", buf);
    close(fd);

    return EXIT_SUCCESS;
}
