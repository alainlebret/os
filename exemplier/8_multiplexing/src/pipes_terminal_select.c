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
#include <unistd.h>     /* pipe(), fork(), read(), write(), close(), sleep() */
#include <stdio.h>      /* dprintf(), perror() */
#include <stdlib.h>     /* exit() */
#include <errno.h>      /* errno, EINTR */
#include <sys/types.h>  /* ssize_t, pid_t */
#include <sys/select.h> /* select(), fd_set, FD_ZERO, FD_SET, FD_ISSET */
#include <sys/wait.h>   /* wait() */

/**
 * @file pipes_terminal_select.c
 * @brief Running example, version 2: reads two pipes and the terminal with
 * select().
 *
 * Lecture: chapter os-07 « Multiplexage des entrées/sorties »
 * (running example "Version 2 – select()").
 *
 * Two children write 5 messages each into their own pipe (child A every
 * 3 seconds, child B every second). The parent waits for the three sources
 * (pipe of A, pipe of B, terminal) with a single select() call and copies
 * each message as soon as it arrives, whatever its source.
 *
 * Key points:
 *   - the fd_set is modified by select(): it is rebuilt before each call,
 *     as is nfds (highest descriptor + 1);
 *   - a descriptor at EOF is always ready: it is closed and removed (-1),
 *     otherwise the loop would become a busy loop;
 *   - the program ends when A and B have finished and Ctrl-D is typed.
 *
 * \code{.bash}
 *   $ ./pipes_terminal_select
 *   [B] message 1
 *   [B] message 2
 *   [A] message 1
 *   [B] message 3
 *   bonjour
 *   bonjour
 *   ...
 * \endcode
 */

#define NB 3

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Child code: writes 5 messages into the pipe, one every 'delai'
 * seconds, then terminates.
 */
void producteur(int tube[2], const char *nom, int delai) {
    close(tube[0]);          /* child: writer */
    for (int i = 1; i <= 5; i++) {
        sleep(delai);
        dprintf(tube[1], "[%s] message %d\n", nom, i);
    }
    exit(EXIT_SUCCESS);      /* closes tube[1] */
}

/**
 * @brief Reads fd once and copies what was read to the standard output.
 * @return 0 on EOF, -1 on error, otherwise the number of bytes read.
 */
ssize_t copier(int fd) {
    char buf[256];
    ssize_t n = read(fd, buf, sizeof buf);
    if (n > 0) {
        if (write(STDOUT_FILENO, buf, n) == -1) {
            return -1;
        }
    }
    return n;
}

/**
 * @brief Rebuilds the set of descriptors to watch.
 * @return The highest descriptor of the set (-1 if the set is empty).
 */
int preparer(fd_set *rfds, const int fds[]) {
    int max = -1;
    FD_ZERO(rfds);
    for (int i = 0; i < NB; i++) {
        if (fds[i] != -1) {          /* -1: removed */
            FD_SET(fds[i], rfds);
            if (fds[i] > max) {
                max = fds[i];
            }
        }
    }
    return max;
}

int main(void) {
    int tube_a[2], tube_b[2];
    pid_t pid;

    if (pipe(tube_a) == -1) {
        handle_fatal_error_and_exit("pipe");
    }
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {
        producteur(tube_a, "A", 3);   /* does not return */
    }
    close(tube_a[1]);                 /* parent: reader */

    if (pipe(tube_b) == -1) {
        handle_fatal_error_and_exit("pipe");
    }
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {
        producteur(tube_b, "B", 1);
    }
    close(tube_b[1]);

    int fds[NB] = { tube_a[0], tube_b[0], STDIN_FILENO };
    int ouverts = NB;                 /* sources still open */

    while (ouverts > 0) {
        fd_set rfds;
        int max = preparer(&rfds, fds);   /* at each iteration */
        if (select(max + 1, &rfds, NULL, NULL, NULL) == -1) {
            if (errno == EINTR) {
                continue;             /* signal received: resume */
            }
            perror("select");
            break;
        }
        for (int i = 0; i < NB; i++) {
            if (fds[i] == -1 || !FD_ISSET(fds[i], &rfds)) {
                continue;
            }
            if (copier(fds[i]) <= 0) {    /* EOF or error */
                close(fds[i]);
                fds[i] = -1;              /* removal */
                ouverts--;
            }
        }
    }

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
