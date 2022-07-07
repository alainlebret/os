/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Files, pipes and fifo" / "Fichiers et entrées-sorties"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file anonymous_pipe_01.c
 * 
 * A simple program that uses an anonymous pipe between a parent and its child.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <sys/types.h> /* wait() */
#include <sys/wait.h> /* wait() */
#include <ctype.h>

#define INPUT 1
#define OUTPUT 0
#define KEYBOARD 0
#define BYTE_SIZE 1

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. It reads characters from keyboard ans writes
 * them in the pipe.
 * @param pipe The anonymous pipe descriptors
 */
void manage_parent(int pipe[])
{
    char byte;

    printf("Parent process (PID %d)\n", getpid());
    close(pipe[OUTPUT]);

    while (read(KEYBOARD, &byte, BYTE_SIZE) > 0) {
        if (isalnum(byte)) {
            write(pipe[1], &byte, 1);
        }
    }
    close(pipe[INPUT]);

    wait(NULL);
    printf("Parent: has received child termination.\n");
}

/**
 * Manages the child process. Reads bytes from the pipe, counts letters and
 * numeral digits and displays the result.
 * @param pipe The anonymous pipe descriptors
 */
void manage_child(int pipe[])
{
    char byte;
    int letters = 0;
    int digits = 0;

    printf("Child process (PID %d)\n", getpid());
    printf("Enter C-D to end.\n");
    close(pipe[INPUT]);

    while (read(pipe[OUTPUT], &byte, BYTE_SIZE) > 0) {
        if (isdigit(byte)) {
            digits++;
        } else {
            letters++;
        }
    }
    printf("\n%d digits received\n", digits);
    printf("%d letters received\n", letters);
}

int main(void)
{
    pid_t pid;
    int anonymous_pipe[2]; /* pipe descriptors */

    if (pipe(anonymous_pipe) == -1) {
        handle_fatal_error("Error creating pipe.\n");
    }

    pid = fork();
    if (pid < 0) {
        handle_fatal_error("Error using fork().\n");
    }
    if (pid > 0) {
        manage_parent(anonymous_pipe);
    } else {
        manage_child(anonymous_pipe);
    }

    exit(EXIT_SUCCESS);
}
