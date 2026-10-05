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
#include <unistd.h>    /* pipe(), dup2(), fork(), close(), read() */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <sys/wait.h>  /* wait() */

/**
 * @file dup_03.c
 * @brief Demonstrates pipe() + dup2(): replicating the shell pipe operator.
 *
 * This is exactly how a shell implements "cmd1 | cmd2":
 *   1. Create a pipe.
 *   2. Child  (cmd1): dup2 the write-end over stdout, then write.
 *   3. Parent (cmd2): read from the read-end (a shell would also dup2 it
 *      over stdin before exec-ing cmd2).
 *
 * Closing the unused pipe ends is essential: the reader only gets EOF once
 * every write-end is closed.
 *
 * \code{.bash}
 *   $ ./dup_03
 *   Parent received: Hello from the child via a pipe!
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

/**
 * @brief Manages the child process.
 *
 * Redirects stdout to the pipe write-end, then writes a message.
 * @param pipefd The pipe file descriptors.
 */
void manage_child(int pipefd[2]) {
    close(pipefd[0]); /* Child does not read from the pipe */

    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        handle_fatal_error_and_exit("dup2");
    }
    close(pipefd[1]);

    /* stdout now points to the pipe write-end */
    printf("Hello from the child via a pipe!\n");
    exit(EXIT_SUCCESS);
}

/**
 * @brief Manages the parent process.
 *
 * Reads the child's message from the pipe read-end and prints it.
 * @param pipefd The pipe file descriptors.
 */
void manage_parent(int pipefd[2]) {
    char buffer[BUFFER_SIZE];
    size_t total = 0;
    ssize_t n = 0;

    close(pipefd[1]); /* Parent does not write to the pipe */

    /* Read until EOF (read() returns 0): the message may come in pieces */
    while (total < sizeof(buffer) - 1
           && (n = read(pipefd[0], buffer + total, sizeof(buffer) - 1 - total)) > 0) {
        total += (size_t) n;
    }
    if (n == -1) {
        handle_fatal_error_and_exit("read");
    }
    buffer[total] = '\0';
    close(pipefd[0]);

    printf("Parent received: %s", buffer);
    if (wait(NULL) == -1) {
        perror("wait");
    }
}

int main(void) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        handle_fatal_error_and_exit("pipe");
    }

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }

    if (pid == 0) {
        manage_child(pipefd);
    } else {
        manage_parent(pipefd);
    }

    return EXIT_SUCCESS;
}
