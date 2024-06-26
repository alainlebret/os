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
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_02.c
 *
 * @brief This program demonstrates process cloning using fork() and explores
 * how a variable's value changes independently in the parent and child 
 * processes.
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process by modifying value of the given parameter.
 * @param parameter Pointer to the integer to modify.
 */
void manage_parent(int32_t *parameter) {
    printf("Parent process (PID %d) begins...\n", getpid());
    *parameter = 10;
    wait(NULL);  /* Wait for the child process to exit */
    printf("Parent process (PID %d) ends with variable: %d\n", getpid(), *parameter);
}

/**
 * @brief Manages the child process by modifying value of the given parameter.
 * @param parameter Pointer to the integer to modify.
 */
void manage_child(int32_t *parameter) {
    printf("Child process (PID %d) begins...\n", getpid());
    *parameter = 20;
    printf("Child process (PID %d) ends with variable: %d\n", getpid(), *parameter);
}

int main(void) {
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

    return EXIT_SUCCESS;
}
