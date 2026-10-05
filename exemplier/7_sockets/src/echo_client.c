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
#include <stdio.h>      /* fgets(), printf(), fprintf(), perror() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* memset(), strlen() */
#include <signal.h>     /* signal(), SIGPIPE */
#include <unistd.h>     /* read(), write(), close() */
#include <arpa/inet.h>  /* htons(), inet_pton() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <sys/socket.h> /* socket(), connect() */

/**
 * @file echo_client.c
 * @brief Running example: TCP client of the echo server (port 5001).
 *
 * Lecture: chapter os-11 « Sockets » ("Fil rouge – client (1/2)" and
 * "(2/2)", "Lire une ligne : lire_ligne()" in the appendices).
 *
 * The client sends each line read on the standard input to the server,
 * reads the echo up to '\n' and displays it. Ctrl-D closes the connection:
 * the server reads EOF.
 *
 * Key points:
 *   - the answer is read up to '\n', not in a single read(): TCP is a byte
 *     stream;
 *   - lire_ligne() makes one system call per byte: simple, but slow;
 *   - with a host name rather than an address, use getaddrinfo() (slide
 *     "getaddrinfo() – essayer chaque adresse").
 * The port can be changed at compile time: -DPORT=5002.
 *
 * \code{.bash}
 *   $ ./echo_client 127.0.0.1
 *   bonjour
 *   écho : bonjour
 *   ^D
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

/**
 * @brief Writes the n bytes of buf.
 * @return n, or -1 on error.
 */
ssize_t ecrire_n(int fd, const char *buf, size_t n) {
    size_t fait = 0;
    while (fait < n) {
        ssize_t r = write(fd, buf + fait, n - fait);
        if (r == -1) {
            return -1;
        }
        fait += r;
    }
    return fait;
}

/**
 * @brief Reads up to '\n' included.
 * @return The length of the line, 0 on immediate EOF.
 */
size_t lire_ligne(int fd, char *buf, size_t max) {
    size_t i = 0;
    char c;
    while (i < max - 1) {
        if (read(fd, &c, 1) != 1) {
            break;             /* EOF or error */
        }
        buf[i++] = c;
        if (c == '\n') {
            break;
        }
    }
    buf[i] = '\0';
    return i;
}

int main(int argc, char *argv[]) {
    struct sockaddr_in srv;
    char ligne[512];

    if (argc != 2) {
        fprintf(stderr, "usage : %s ip\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
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
    if (connect(fd, (struct sockaddr *) &srv, sizeof srv) == -1) {
        erreur("connect");
    }
    signal(SIGPIPE, SIG_IGN);      /* server left: EPIPE */

    while (fgets(ligne, sizeof ligne, stdin) != NULL) {
        if (ecrire_n(fd, ligne, strlen(ligne)) == -1) {
            break;                 /* server left */
        }
        if (lire_ligne(fd, ligne, sizeof ligne) == 0) {
            break;                 /* server left */
        }
        printf("écho : %s", ligne);
    }
    close(fd);                     /* EOF for the server */

    return EXIT_SUCCESS;
}
