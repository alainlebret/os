/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <sys/types.h> /* wait() */
#include <sys/wait.h> /* wait() */
#include <ctype.h>

/**
 * @file anonymous_pipe_01.c
 *
 * A simple program that uses an anonymous pipe between a parent and its child.
 */

#define PIPE_INPUT  1
#define PIPE_OUTPUT 0
#define KEYBOARD    0
#define BYTE_SIZE   1

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. It reads characters from keyboard ans writes
 * them in the pipe.
 * @param pipe The anonymous pipe descriptors.
 * @param pid_child The PID of the child to send values.
 */
void manage_parent(int pipe[]) {
    char byte;
    ssize_t ret;

    printf("Parent process (PID %d)\n", getpid());
    close(pipe[PIPE_OUTPUT]);

    do {
        ret = read(KEYBOARD, &byte, BYTE_SIZE);
        if (ret == -1) {
            perror("Error reading from keyboard");
            break;
        } else if (ret != 0) {
            if (isalnum(byte)) {
                if (write(pipe[PIPE_INPUT], &byte, BYTE_SIZE) == -1) {
                    perror("Error writing to pipe");
                    break;
                }
            }
        }
    } while (ret > 0);
    close(pipe[PIPE_INPUT]);

    wait(NULL);
    printf("Parent: has received child termination.\n");
}

/**
 * Manages the child process. Reads bytes from the pipe, counts letters and
 * numeral digits and displays the result.
 * @param pipe The anonymous pipe descriptors
 */
void manage_child(int pipe[]) {
    char byte;
    int letters = 0, digits = 0;
    ssize_t ret;

    printf("Child process (PID %d)\n", getpid());
    printf("Enter Ctrl-D (EOF) to end.\n");
    close(pipe[PIPE_INPUT]);

    while ((ret = read(pipe[PIPE_OUTPUT], &byte, BYTE_SIZE)) > 0) {
        if (isdigit(byte)) {
            digits++;
        } else if (isalpha(byte)) {
            letters++;
        }
    }
    if (ret == -1) {
        perror("Error reading from pipe");
    }

    printf("\n%d digits received\n", digits);
    printf("%d letters received\n", letters);
}

int main(void) {
    pid_t pid;
    int anonymous_pipe[2]; /* pipe descriptors */

    if (pipe(anonymous_pipe) == -1) {
        handle_fatal_error("Error [pipe()]: ");
    }

    pid = fork();
    if (pid == -1) {
        handle_fatal_error("Error [fork()]: ");
    }
    if (pid > 0) {
        manage_parent(anonymous_pipe);
    } else {
        manage_child(anonymous_pipe);
    }

    return EXIT_SUCCESS;
}
