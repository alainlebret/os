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
#include <unistd.h>     /* pipe(), fork(), read(), close(), write() */
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* exit() */
#include <sys/select.h> /* select(), fd_set, FD_ZERO, FD_SET, FD_ISSET */
#include <sys/time.h>   /* struct timeval */
#include <sys/wait.h>   /* wait() */

/**
 * @file select_02.c
 * @brief Demonstrates select() monitoring two pipes simultaneously.
 *
 * Two child processes each write a message to their own pipe after a
 * different delay. The parent uses select() to read from whichever pipe
 * becomes ready first, without blocking on the slower one.
 *
 * Key points:
 *   - fd_set and nfds must be rebuilt before each select() call.
 *   - The loop ends once both pipes have been drained.
 *
 * This pattern is fundamental for servers handling multiple clients.
 *
 * \code{.bash}
 *   $ ./select_02
 *   Ready: pipe from child 1 — message: "Data from child 1"
 *   Ready: pipe from child 2 — message: "Data from child 2"
 * \endcode
 */

#define BUFFER_SIZE 128

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    int pipe1[2], pipe2[2];
    fd_set readfds;
    char buffer[BUFFER_SIZE];
    int nfds, ready, done;
    ssize_t n;
    pid_t pid;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        handle_fatal_error_and_exit("pipe");
    }

    /* Child 1: writes after 1 second */
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {
        close(pipe1[0]); close(pipe2[0]); close(pipe2[1]);
        sleep(1);
        if (write(pipe1[1], "Data from child 1", 17) == -1) {
            handle_fatal_error_and_exit("write");
        }
        close(pipe1[1]);
        exit(EXIT_SUCCESS);
    }

    /* Child 2: writes after 2 seconds */
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {
        close(pipe2[0]); close(pipe1[0]); close(pipe1[1]);
        sleep(2);
        if (write(pipe2[1], "Data from child 2", 17) == -1) {
            handle_fatal_error_and_exit("write");
        }
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    }

    /* Parent: close write-ends */
    close(pipe1[1]);
    close(pipe2[1]);

    nfds = (pipe2[0] > pipe1[0] ? pipe2[0] : pipe1[0]) + 1;
    done = 0;

    while (done < 2) {
        /* Rebuild fd_set before every select() call */
        FD_ZERO(&readfds);
        if (pipe1[0] != -1) FD_SET(pipe1[0], &readfds);
        if (pipe2[0] != -1) FD_SET(pipe2[0], &readfds);

        ready = select(nfds, &readfds, NULL, NULL, NULL);
        if (ready == -1) {
            handle_fatal_error_and_exit("select");
        }

        if (pipe1[0] != -1 && FD_ISSET(pipe1[0], &readfds)) {
            n = read(pipe1[0], buffer, sizeof(buffer) - 1);
            if (n == -1) {
                handle_fatal_error_and_exit("read");
            }
            buffer[n] = '\0';
            printf("Ready: pipe from child 1 — message: \"%s\"\n", buffer);
            close(pipe1[0]);
            pipe1[0] = -1;
            done++;
        }
        if (pipe2[0] != -1 && FD_ISSET(pipe2[0], &readfds)) {
            n = read(pipe2[0], buffer, sizeof(buffer) - 1);
            if (n == -1) {
                handle_fatal_error_and_exit("read");
            }
            buffer[n] = '\0';
            printf("Ready: pipe from child 2 — message: \"%s\"\n", buffer);
            close(pipe2[0]);
            pipe2[0] = -1;
            done++;
        }
    }

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
