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

#include <stdint.h>    /* C99 int types */
#include <inttypes.h>  /* C99 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.1
 * @date 2017-12-31
 */

/**
 * @file process_02.c
 *
 * A simple program that clones a process using the \c fork() primitive and
 * shows the evolution of a variable in parent and child processes.
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
 * @brief Manages the parent process by modifying value of the given parameter.
 */
void manage_parent(int32_t *parameter)
{
    printf("Parent process (PID %"
    PRId32
    ")\n", getpid());
    printf("Parent modifies its own variable...\n");
    *parameter = 10;
    wait(NULL);
}

/**
 * @brief Manages the child process by modifying value of the given parameter.
 */
void manage_child(int32_t *parameter)
{
    printf("Child process (PID %"
    PRId32
    ")\n", getpid());
    printf("Child modifies its own variable...\n");
    *parameter = 20;
}

int main(void)
{
    pid_t pid;
    int32_t own_variable = 0;

    pid = fork();
    if (pid < 0) {
        handle_fatal_error_and_exit("Error using fork().\n");
    }

    if (pid > 0) {
        manage_parent(&own_variable);
    } else {
        manage_child(&own_variable);
    }
    printf("\nPID %"
    PRId32
    " has its own variable equals to: %"
    PRId32
    "\n", getpid(), own_variable);

    exit(EXIT_SUCCESS);
}
