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
#include <stdio.h>      /* perror() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* memset() */
#include <errno.h>      /* errno */
#include <signal.h>     /* sigaction(), signal(), SIGCHLD, SIGPIPE */
#include <stdint.h>     /* uint16_t */
#include <unistd.h>     /* read(), write(), close(), fork() */
#include <arpa/inet.h>  /* htons(), htonl() */
#include <netinet/in.h> /* struct sockaddr_in, INADDR_ANY */
#include <sys/socket.h> /* socket(), bind(), listen(), accept(), setsockopt() */
#include <sys/wait.h>   /* waitpid(), WNOHANG */

/**
 * @file echo_server_concurrent.c
 * @brief Running example: concurrent TCP echo server on port 5001, one
 * child process per client.
 *
 * Lecture: chapter os-11 « Sockets » ("Fil rouge – serveur concurrent",
 * "Récolter les enfants : SIGCHLD", "SO_REUSEADDR").
 *
 * The parent (the "veilleur") only accepts connections; each client is
 * served by a child process.
 *
 * Key points:
 *   - the child closes the listening socket, the parent closes the
 *     communication socket; otherwise the connection would not be closed
 *     when the client leaves;
 *   - the children are reaped in a SIGCHLD handler, with waitpid(-1, NULL,
 *     WNOHANG) in a loop, otherwise they would remain zombies;
 *   - SA_RESTART restarts accept() interrupted by the signal.
 * The port can be changed at compile time: -DPORT=5002.
 *
 * \code{.bash}
 *   $ ./echo_server_concurrent &
 *   $ nc 127.0.0.1 5001        # several clients at the same time
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
 * @brief SIGCHLD handler: reaps every finished child.
 */
void sur_sigchld(int sig) {
    int sauve = errno;         /* waitpid() modifies errno */
    (void) sig;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        ;                      /* each finished child */
    }
    errno = sauve;
}

int main(void) {
    struct sigaction sa;

    memset(&sa, 0, sizeof sa);
    sa.sa_handler = sur_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;      /* restart accept() */
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        erreur("sigaction");
    }
    signal(SIGPIPE, SIG_IGN);      /* EPIPE */

    int ecoute = creer_ecoute(PORT);
    for (;;) {
        int com = accept(ecoute, NULL, NULL);
        if (com == -1) {
            if (errno != EINTR) {  /* EINTR: interrupted by SIGCHLD, not an error */
                perror("accept");
            }
            continue;
        }
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
        } else if (pid == 0) {     /* child */
            close(ecoute);         /* does not accept */
            servir(com);
            close(com);
            exit(EXIT_SUCCESS);
        }
        close(com);                /* parent */
    }
}
