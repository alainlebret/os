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

#include <unistd.h>     /* read(), STDIN_FILENO */
#include <stdio.h>      /* printf(), fflush() */
#include <stdlib.h>     /* exit() */
#include <sys/select.h> /* select(), fd_set, FD_ZERO, FD_SET, FD_ISSET */
#include <sys/time.h>   /* struct timeval */

/**
 * @file select_01.c
 * @brief Demonstrates select() with a timeout on standard input.
 *
 * select() watches a set of file descriptors, blocking until at least one
 * becomes ready for I/O or a timeout expires. Here it monitors stdin for
 * up to 5 seconds. If the user types within that window the input is echoed;
 * otherwise a timeout message is printed.
 *
 * \code{.bash}
 *   $ ./select_01
 *   Waiting up to 5 seconds for input on stdin...
 *   hello
 *   You typed: hello
 *
 *   $ ./select_01         (no input within 5 seconds)
 *   Waiting up to 5 seconds for input on stdin...
 *   Timeout: no input received.
 * \endcode
 */

#define TIMEOUT_SECONDS 5
#define BUFFER_SIZE     128

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    fd_set readfds;
    struct timeval timeout;
    char buffer[BUFFER_SIZE];
    int ready;
    ssize_t n;

    printf("Waiting up to %d seconds for input on stdin...\n", TIMEOUT_SECONDS);
    fflush(stdout);

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    /* select() may modify timeout: reinitialise before each call */
    timeout.tv_sec  = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
    if (ready == -1) {
        handle_fatal_error_and_exit("select");
    }

    if (ready == 0) {
        printf("Timeout: no input received.\n");
        return EXIT_SUCCESS;
    }

    /* STDIN_FILENO is ready for reading */
    n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        handle_fatal_error_and_exit("read");
    }
    buffer[n] = '\0';
    printf("You typed: %s", buffer);

    return EXIT_SUCCESS;
}
