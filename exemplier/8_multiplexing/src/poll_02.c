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
#include <unistd.h>  /* pipe(), fork(), read(), close(), write(), sleep() */
#include <stdio.h>   /* printf(), dprintf() */
#include <stdlib.h>  /* exit() */
#include <poll.h>    /* poll(), struct pollfd, POLLIN, POLLHUP, POLLERR */
#include <sys/wait.h> /* wait() */

/**
 * @file poll_02.c
 * @brief Demonstrates poll() monitoring two pipes simultaneously.
 *
 * Functionally identical to select_02.c but uses poll(). Key differences
 * from the select() version:
 *   - No fd_set rebuild: the pollfd array is stable across iterations.
 *   - Removing a descriptor from monitoring is done by setting .fd = -1;
 *     poll() then skips that entry.
 *   - Scales to many descriptors without the FD_SETSIZE limit of select().
 *
 * \code{.bash}
 *   $ ./poll_02
 *   Ready: pipe 0 — message: "Data from child 1"
 *   Ready: pipe 1 — message: "Data from child 2"
 * \endcode
 */

#define BUFFER_SIZE 128
#define NUM_PIPES   2

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    int pipes[NUM_PIPES][2];
    struct pollfd fds[NUM_PIPES];
    char buffer[BUFFER_SIZE];
    int i, j, ready, done;
    ssize_t n;
    pid_t pid;

    for (i = 0; i < NUM_PIPES; i++) {
        if (pipe(pipes[i]) == -1) {
            handle_fatal_error_and_exit("pipe");
        }
    }

    /* Child i writes to pipe i after (i+1) seconds */
    for (i = 0; i < NUM_PIPES; i++) {
        pid = fork();
        if (pid == -1) {
            handle_fatal_error_and_exit("fork");
        }
        if (pid == 0) {
            for (j = 0; j < NUM_PIPES; j++) {
                close(pipes[j][0]);
                if (j != i) close(pipes[j][1]);
            }
            sleep(i + 1);
            if (dprintf(pipes[i][1], "Data from child %d", i + 1) == -1) {
                handle_fatal_error_and_exit("dprintf");
            }
            close(pipes[i][1]);
            exit(EXIT_SUCCESS);
        }
    }

    /* Parent: close write-ends, set up pollfd array */
    for (i = 0; i < NUM_PIPES; i++) {
        close(pipes[i][1]);
        fds[i].fd     = pipes[i][0];
        fds[i].events = POLLIN;
    }

    done = 0;
    while (done < NUM_PIPES) {
        ready = poll(fds, NUM_PIPES, -1); /* -1 = no timeout */
        if (ready == -1) {
            handle_fatal_error_and_exit("poll");
        }

        for (i = 0; i < NUM_PIPES; i++) {
            if (fds[i].fd == -1) {
                continue;
            }
            /* Error on the descriptor: removed without reading */
            if (fds[i].revents & (POLLERR | POLLNVAL)) {
                close(fds[i].fd);
                fds[i].fd = -1;
                done++;
                continue;
            }
            /* POLLHUP (child gone) may be set without POLLIN: read() then
             * returns 0 (EOF) and the pipe is removed as well, otherwise
             * poll() would return at once forever */
            if (!(fds[i].revents & (POLLIN | POLLHUP))) {
                continue;
            }
            n = read(fds[i].fd, buffer, sizeof(buffer) - 1);
            if (n == -1) {
                handle_fatal_error_and_exit("read");
            }
            buffer[n] = '\0';
            printf("Ready: pipe %d — message: \"%s\"\n", i, buffer);
            close(fds[i].fd);
            fds[i].fd = -1; /* Remove from future poll() calls */
            done++;
        }
    }

    for (i = 0; i < NUM_PIPES; i++) {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
