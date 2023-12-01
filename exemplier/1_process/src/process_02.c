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

#include <stdint.h>    /* C99 int types */
#include <inttypes.h>  /* C99 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_02.c
 * @brief This program demonstrates process cloning using fork() and explores
 * how a variable's value changes independently in the parent and child 
 * processes.
 *
 * @author Alain Lebret
 * @version	1.1
 * @date 2017-12-31
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process by modifying value of the given parameter.
 */
void manage_parent(int32_t *parameter)
{
    printf("Parent process (PID %" PRId32 ")\n", getpid());
    printf("Parent modifies its own variable...\n");
    *parameter = 10;
    wait(NULL);
}

/**
 * @brief Manages the child process by modifying value of the given parameter.
 */
void manage_child(int32_t *parameter)
{
    printf("Child process (PID %" PRId32 ")\n", getpid());
    printf("Child modifies its own variable...\n");
    *parameter = 20;
}

int main(void)
{
    pid_t pid;
    int32_t own_variable = 0;

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    }

    if (pid > 0) {
        manage_parent(&own_variable);
    } else {
        manage_child(&own_variable);
    }
    printf("\nPID %" PRId32 " has its own variable equals to: %" PRId32 "\n", getpid(), own_variable);

    return EXIT_SUCCESS;
}
