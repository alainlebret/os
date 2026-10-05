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
#include <errno.h>      /* errno, EINTR */
#include <poll.h>       /* poll(), struct pollfd, POLLIN, POLLHUP */
#include <signal.h>     /* signal(), SIGPIPE */
#include <stdint.h>     /* uint16_t */
#include <unistd.h>     /* read(), write(), close() */
#include <arpa/inet.h>  /* htons(), htonl() */
#include <netinet/in.h> /* struct sockaddr_in, INADDR_ANY */
#include <sys/socket.h> /* socket(), bind(), listen(), accept(), setsockopt() */

/**
 * @file echo_server_poll.c
 * @brief TCP echo server on port 5001 multiplexing all its clients with
 * poll(), in a single process.
 *
 * Lecture: chapter os-11 « Sockets » ("Multiplexer les requêtes" and, in
 * the appendices, "Multiplexer les requêtes : squelette"); see also
 * chapter os-07 « Multiplexage des entrées/sorties ».
 *
 * Element 0 of the pollfd array is the listening socket: it is ready for
 * reading when a connection waits for accept(). The other elements are the
 * communication sockets: ready when the client has sent data, or closed.
 *
 * Key points:
 *   - no process to create nor to reap;
 *   - but a long processing blocks all the clients;
 *   - a client at EOF is removed from the array (the last element takes
 *     its place, hence i-- to examine it).
 * The port can be changed at compile time: -DPORT=5002.
 *
 * \code{.bash}
 *   $ ./echo_server_poll &
 *   $ nc 127.0.0.1 5001        # several clients at the same time
 *   salut
 *   salut
 * \endcode
 */

#ifndef PORT
#define PORT 5001
#endif

#define MAX 64                  /* listening socket + 63 clients */

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
 * @brief Creates the listening socket; exits on error.
 */
int creer_ecoute(uint16_t port) {
    struct sockaddr_in adr;
    int oui = 1;
    int ecoute = socket(AF_INET, SOCK_STREAM, 0);
    if (ecoute == -1) {
        erreur("socket");
    }
    if (setsockopt(ecoute, SOL_SOCKET, SO_REUSEADDR,
                   &oui, sizeof oui) == -1) {
        erreur("setsockopt");
    }
    memset(&adr, 0, sizeof adr);
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ecoute, (struct sockaddr *) &adr, sizeof adr) == -1) {
        erreur("bind");
    }
    if (listen(ecoute, 10) == -1) {
        erreur("listen");
    }
    return ecoute;
}

/**
 * @brief Adds the communication socket com to the array (closes it if the
 * array is full or if accept() failed).
 */
void ajouter(struct pollfd fds[], int *nb, int com) {
    if (com == -1) {
        return;
    }
    if (*nb == MAX) {
        close(com);             /* too many clients */
        return;
    }
    fds[*nb].fd = com;
    fds[*nb].events = POLLIN;
    fds[*nb].revents = 0;
    (*nb)++;
}

/**
 * @brief Closes the socket of element i and replaces it by the last one.
 */
void retirer(struct pollfd fds[], int *nb, int i) {
    close(fds[i].fd);
    fds[i] = fds[*nb - 1];
    (*nb)--;
}

/**
 * @brief One read(), then sends back what was read.
 * @return The result of read(): 0 on EOF, -1 on error.
 */
ssize_t echo_une_fois(int fd) {
    char buf[512];
    ssize_t n = read(fd, buf, sizeof buf);
    if (n > 0 && ecrire_n(fd, buf, n) == -1) {
        return -1;              /* client left */
    }
    return n;
}

int main(void) {
    int ecoute = creer_ecoute(PORT);
    signal(SIGPIPE, SIG_IGN);           /* EPIPE */

    struct pollfd fds[MAX] = {
        { .fd = ecoute, .events = POLLIN, .revents = 0 },
    };
    int nb = 1;

    for (;;) {
        if (poll(fds, nb, -1) == -1) {
            if (errno == EINTR) {
                continue;
            }
            erreur("poll");
        }
        if (fds[0].revents & POLLIN) {      /* new client */
            ajouter(fds, &nb, accept(ecoute, NULL, NULL));
        }
        for (int i = 1; i < nb; i++) {
            if (fds[i].revents & (POLLERR | POLLNVAL)) {  /* error: no read */
                retirer(fds, &nb, i--);
                continue;
            }
            if (fds[i].revents & (POLLIN | POLLHUP)) {
                if (echo_une_fois(fds[i].fd) <= 0) {
                    retirer(fds, &nb, i--);  /* EOF */
                }
            }
        }
    }
}
