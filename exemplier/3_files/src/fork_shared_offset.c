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
#include <fcntl.h>     /* open() */
#include <unistd.h>    /* fork(), read(), write(), close() */
#include <stdio.h>     /* printf(), perror() */
#include <stdlib.h>    /* exit() */
#include <string.h>    /* strlen() */
#include <sys/wait.h>  /* wait() */

/**
 * @file fork_shared_offset.c
 * @brief After fork(), parent and child share the file offset.
 *
 * Course "Operating Systems", chapter « Fichiers et descripteurs »,
 * exercise 1 (« position partagée »).
 *
 * fork() copies the file descriptor table, not the open file description:
 * both processes therefore share the same current position. The child
 * reads "abc", so the parent then reads from offset 3 and prints "def".
 *
 * \code{.bash}
 *   $ ./fork_shared_offset
 *   def
 * \endcode
 */

#define FILENAME "f.txt"

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Creates the file used by the exercise, containing "abcdef".
 */
void create_test_file(void) {
    const char *content = "abcdef";
    int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        handle_fatal_error_and_exit("open (creation)");
    }
    if (write(fd, content, strlen(content)) == -1) {
        handle_fatal_error_and_exit("write");
    }
    close(fd);
}

int main(void) {
    pid_t pid;

    create_test_file();

    /* Code of the exercise */
    int fd = open(FILENAME, O_RDONLY);
    char buf[4] = "";

    if (fd == -1) {
        handle_fatal_error_and_exit("open");
    }

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {             /* child */
        if (read(fd, buf, 3) == -1) {
            perror("read (child)");
        }
        exit(EXIT_SUCCESS);
    }
    if (wait(NULL) == -1) {     /* parent */
        handle_fatal_error_and_exit("wait");
    }
    if (read(fd, buf, 3) == -1) {
        handle_fatal_error_and_exit("read (parent)");
    }
    printf("%s\n", buf);

    close(fd);
    return EXIT_SUCCESS;
}
