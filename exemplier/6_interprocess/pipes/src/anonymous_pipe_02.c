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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* wait() */
#include <sys/wait.h>  /* wait() */
#include <ctype.h>

/**
 * @file anonymous_pipe_02.c
 *
 * Another simple program that uses an anonymous pipe between a parent and its
 * child.
 */

#define PIPE_INPUT    1
#define PIPE_OUTPUT   0
#define KEYBOARD      0
#define STOP         -1
#define INTEGER_SIZE  sizeof(int)
#define BUFFER_SIZE   100

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. It reads integers from keyboard ans writes
 * them in the pipe. It sends -1 to stop communication.
 * @param pipe The anonymous pipe descriptors
 */
void manage_parent(int pipe[]) {
    int integer;
    char buffer[BUFFER_SIZE];

    printf("Parent process (PID %d)\n", getpid());
    close(pipe[PIPE_OUTPUT]);

    while (1) {
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break; /* Break on EOF or error */
        }
        if (sscanf(buffer, "%d", &integer) != 1) {
            fprintf(stderr, "Invalid input\n");
            continue;
        }
        if (write(pipe[PIPE_INPUT], &integer, INTEGER_SIZE) == -1) {
            perror("Error writing to pipe");
            break;
        }
        if (integer == STOP) {
            break;
        }
    }
    close(pipe[PIPE_INPUT]);

    wait(NULL);
    printf("Parent: has received child termination.\n");
}

/**
 * Manages the child process. Reads integers from the pipe and displays them.
 * It stops when receiving -1.
 * @param pipe The anonymous pipe descriptors
 */
void manage_child(int pipe[]) {
    int integer, bytesRead;

    printf("Child process (PID %d)\n", getpid());
    close(pipe[PIPE_INPUT]);

    while (1) {
        bytesRead = read(pipe[PIPE_OUTPUT], &integer, INTEGER_SIZE);
        if (bytesRead == -1) {
            perror("Error reading from pipe");
            break;
        }
        if (bytesRead == 0) { /* Pipe is closed */
            break;
        }
        printf("\nValue received: %d\n", integer);
        if (integer == STOP) {
            break;
        }
    }
    close(pipe[PIPE_OUTPUT]);
    printf("Child has finished.\n");
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

