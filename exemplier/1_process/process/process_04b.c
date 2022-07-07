/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Processes & signals" / "Processus et signaux"
 *
 * Copyright (C) 1995-2018 Alain Lebret (alain.lebret@ensicaen.fr)
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

#include <inttypes.h>  /* C99 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork() and sleep() */
#include <sys/types.h> /* pid_t */

#define DURATION 20

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.1
 * @date 2017-12-31
 */

/**
 * @file process_04b.c
 *
 * A simple program that clones a process using the \c fork() primitive, and
 * where the parent process dies before his child. The child process becomes
 * orphean.
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process. 
 *
 * The parent process do not wait for his child and it dies.
 */
void manage_parent()
{
    printf("Parent process (PID %"
    PRId32
    ")\n", getpid());
}

/**
 * @brief Manages the child process. 
 *
 * The child process is blocked during \em DURATION seconds.
 */
void manage_child()
{
    printf("Child process (PID %"
    PRId32
    ")\n", getpid());
    printf("Child will be blocked during %"
    PRId32
    " seconds...\n", DURATION);

    sleep(DURATION);

    printf("Child has finished to sleep.\n");
    printf("The PID of my parent is %"
    PRId32
    ". I am an orphean.\n", getppid());
}

int main(void)
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        handle_fatal_error_and_exit("Error using fork().\n");
    }

    if (pid > 0) {
        manage_parent();
    } else {
        manage_child();
    }

    exit(EXIT_SUCCESS);
}
