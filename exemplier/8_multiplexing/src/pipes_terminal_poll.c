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
#include <poll.h>       /* poll(), struct pollfd, POLLIN, POLLERR, POLLNVAL */
#include <sys/types.h>  /* ssize_t, pid_t */
#include <sys/wait.h>   /* wait() */

/**
 * @file pipes_terminal_poll.c
 * @brief Running example, version 3: reads two pipes and the terminal with
 * poll().
 *
 * Lecture: chapter os-07 « Multiplexage des entrées/sorties »
 * (running example "Version 3 – poll()").
 *
 * Same scenario as pipes_terminal_select.c: two children write 5 messages
 * each into their own pipe (A every 3 seconds, B every second) and the
 * parent copies each message as soon as it arrives, as well as the lines
 * typed on the terminal.
 *
 * Key points:
 *   - the pollfd array is filled only once, before the loop: the kernel
 *     only modifies revents;
 *   - no computation of the highest descriptor;
 *   - an entry whose fd is negative is ignored: this is how a source at
 *     EOF (or in POLLERR / POLLNVAL) is removed.
 *
 * \code{.bash}
 *   $ ./pipes_terminal_poll
 *   [B] message 1
 *   [B] message 2
 *   [A] message 1
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

    struct pollfd fds[NB] = {
        { .fd = tube_a[0],    .events = POLLIN, .revents = 0 },
        { .fd = tube_b[0],    .events = POLLIN, .revents = 0 },
        { .fd = STDIN_FILENO, .events = POLLIN, .revents = 0 },
    };
    int ouverts = NB;                 /* sources still open */

    while (ouverts > 0) {
        if (poll(fds, NB, -1) == -1) {    /* -1: infinite */
            if (errno == EINTR) {
                continue;             /* signal received: resume */
            }
            perror("poll");
            break;
        }
        for (int i = 0; i < NB; i++) {
            short ev = fds[i].revents;
            if (ev == 0) {
                continue;
            }
            /* POLLERR, POLLNVAL: unusable fd */
            if ((ev & (POLLERR | POLLNVAL))
                || copier(fds[i].fd) <= 0) {  /* EOF, error */
                close(fds[i].fd);
                fds[i].fd = -1;           /* ignored by poll() */
                ouverts--;
            }
        }
    }

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
