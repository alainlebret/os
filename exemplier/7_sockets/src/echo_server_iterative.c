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
#include <signal.h>     /* signal(), SIGPIPE */
#include <stdint.h>     /* uint16_t */
#include <unistd.h>     /* read(), write(), close() */
#include <arpa/inet.h>  /* htons(), htonl(), ntohs(), inet_ntop() */
#include <netinet/in.h> /* struct sockaddr_in, INADDR_ANY */
#include <sys/socket.h> /* socket(), bind(), listen(), accept(), setsockopt() */

/**
 * @file echo_server_iterative.c
 * @brief Running example: iterative TCP echo server on port 5001.
 *
 * Lecture: chapter os-11 « Sockets » ("Fil rouge – servir un client",
 * "Fil rouge – socket d'écoute", "Fil rouge – serveur itératif").
 *
 * The server sends back to the client everything it receives, until the
 * client closes the connection, then accepts the next client.
 *
 * Key points:
 *   - listening socket != communication socket;
 *   - TCP is a byte stream: write() may write less than requested, hence
 *     ecrire_n();
 *   - SIGPIPE is ignored: writing to a client that left gives EPIPE;
 *   - SO_REUSEADDR (slide "SO_REUSEADDR") allows restarting the server
 *     despite connections in TIME_WAIT;
 *   - limit: while servir() runs, a second client is not served (see
 *     echo_server_concurrent.c).
 * The port can be changed at compile time: -DPORT=5002 (default 5001:
 * on macOS, port 5000 is used by the AirPlay receiver).
 *
 * \code{.bash}
 *   $ ./echo_server_iterative          # terminal 1
 *   client 127.0.0.1:48512
 *   $ nc 127.0.0.1 5001                # terminal 2 (or echo_client)
 *   salut
 *   salut
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
 * @brief Sends back to the client everything it sends, until it closes
 * the connection.
 */
void servir(int com) {
    char buf[512];
    ssize_t n;
    while ((n = read(com, buf, sizeof buf)) > 0) {
        if (ecrire_n(com, buf, n) == -1) {
            break;             /* client left */
        }
    }
    if (n == -1) {
        perror("read");
    }
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

int main(void) {
    int ecoute = creer_ecoute(PORT);
    signal(SIGPIPE, SIG_IGN);      /* EPIPE */
    for (;;) {
        struct sockaddr_in cli;
        socklen_t lg = sizeof cli;
        char ip[INET_ADDRSTRLEN];
        int com = accept(ecoute, (struct sockaddr *) &cli, &lg);
        if (com == -1) {
            /* As in the slide, the server stops; a real server would go on
               after a transient error such as ECONNABORTED */
            erreur("accept");
        }
        inet_ntop(AF_INET, &cli.sin_addr, ip, sizeof ip);
        printf("client %s:%d\n", ip, ntohs(cli.sin_port));
        fflush(stdout);
        servir(com);
        close(com);
    }
}
