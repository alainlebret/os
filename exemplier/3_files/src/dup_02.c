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
#include <unistd.h>    /* dup2(), close(), STDOUT_FILENO */
#include <fcntl.h>     /* open() */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */

/**
 * @file dup_02.c
 * @brief Demonstrates dup2(): redirecting stdout to a file.
 *
 * dup2(oldfd, newfd) atomically closes newfd (if open) and makes it refer
 * to the same file as oldfd. This is the mechanism shells use to implement
 * output redirection: "cmd > file.txt".
 *
 * After the dup2() call, every printf() writes to output.txt instead of
 * the terminal.
 *
 * \code{.bash}
 *   $ ./dup_02
 *   $ cat output.txt
 *   This line goes to the file, not the terminal.
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
    int file_fd;

    file_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1) {
        handle_fatal_error_and_exit("open");
    }

    /* Redirect stdout (fd 1) to output.txt */
    if (dup2(file_fd, STDOUT_FILENO) == -1) {
        handle_fatal_error_and_exit("dup2");
    }
    close(file_fd); /* No longer needed directly */

    /* printf now writes into output.txt */
    printf("This line goes to the file, not the terminal.\n");

    return EXIT_SUCCESS;
}
