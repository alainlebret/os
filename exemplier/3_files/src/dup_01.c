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
#include <unistd.h>    /* dup(), close(), STDOUT_FILENO, dprintf() */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */

/**
 * @file dup_01.c
 * @brief Demonstrates dup(): duplicating a file descriptor.
 *
 * dup() creates a new file descriptor pointing to the same open file
 * description as the given one. The new descriptor is always the lowest
 * available number. Both descriptors share the same file offset and flags.
 *
 * \code{.bash}
 *   $ ./dup_01
 *   Original stdout fd : 1
 *   Duplicate fd       : 3
 *   Written via fd 1: hello from stdout
 *   Written via fd 3: hello from dup
 * \endcode
 */

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    int dup_fd;

    printf("Original stdout fd : %d\n", STDOUT_FILENO);

    dup_fd = dup(STDOUT_FILENO);
    if (dup_fd == -1) {
        handle_fatal_error_and_exit("dup");
    }

    printf("Duplicate fd       : %d\n", dup_fd);

    /* Both point to the same open file description: both write to the terminal */
    dprintf(STDOUT_FILENO, "Written via fd %d: hello from stdout\n", STDOUT_FILENO);
    dprintf(dup_fd,        "Written via fd %d: hello from dup\n",    dup_fd);

    close(dup_fd);
    return EXIT_SUCCESS;
}
