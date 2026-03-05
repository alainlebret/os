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
#include <unistd.h>  /* read(), STDIN_FILENO */
#include <stdio.h>   /* printf(), fflush() */
#include <stdlib.h>  /* exit() */
#include <poll.h>    /* poll(), struct pollfd, POLLIN */

/**
 * @file poll_01.c
 * @brief Demonstrates poll() with a timeout on standard input.
 *
 * poll() is a modern alternative to select(). Instead of three fd_set
 * bitmasks it takes an array of struct pollfd, which scales better to
 * large numbers of descriptors and has a cleaner API.
 *
 * Compare with select_01.c: same behaviour, different API.
 * Key differences from select():
 *   - Timeout is a single integer in milliseconds (no struct timeval).
 *   - No need to rebuild the descriptor set before each call.
 *   - Not limited by FD_SETSIZE.
 *
 * \code{.bash}
 *   $ ./poll_01
 *   Waiting up to 5 seconds for input on stdin...
 *   hello
 *   You typed: hello
 *
 *   $ ./poll_01         (no input within 5 seconds)
 *   Waiting up to 5 seconds for input on stdin...
 *   Timeout: no input received.
 * \endcode
 */

#define TIMEOUT_MS  5000
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
    struct pollfd fds[1];
    char buffer[BUFFER_SIZE];
    int ready;
    ssize_t n;

    printf("Waiting up to %d seconds for input on stdin...\n", TIMEOUT_MS / 1000);
    fflush(stdout);

    fds[0].fd     = STDIN_FILENO;
    fds[0].events = POLLIN; /* Watch for data available to read */

    ready = poll(fds, 1, TIMEOUT_MS);
    if (ready == -1) {
        handle_fatal_error_and_exit("poll");
    }

    if (ready == 0) {
        printf("Timeout: no input received.\n");
        return EXIT_SUCCESS;
    }

    if (fds[0].revents & POLLIN) {
        n = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (n == -1) {
            handle_fatal_error_and_exit("read");
        }
        buffer[n] = '\0';
        printf("You typed: %s", buffer);
    }

    return EXIT_SUCCESS;
}
