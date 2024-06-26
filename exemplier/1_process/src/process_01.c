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

#include <unistd.h>    /* fork() */
#include <stdio.h>     /* printf() and perror() */
#include <stdlib.h>    /* exit() */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_01.c
 * @brief A simple program that demonstrates the use of the fork() system call
 * to create a child process. 
 *
 * It separates the behavior of the parent and child processes into different 
 * functions, printing their respective process IDs and executing distinct 
 * instructions.
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits with EXIT_FAILURE.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process.
 * @param child_pid The PID of the child process.
 */
void manage_parent(pid_t child_pid) {
    printf("Parent process (PID %d)\n", getpid());
    printf("My child's PID is %d\n", child_pid);
    printf("Instructions of parent process...\n");
    wait(NULL); /* Wait for child to exit to prevent zombie process */
}

/**
 * @brief Manages the child process.
 */
void manage_child() {
    printf("Child process (PID %d)\n", getpid());
    printf("My parent's PID is %d\n", getppid());
    printf("Instructions of child process...\n");
}

int main(void) {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    }
    if (pid > 0) {
        manage_parent(pid);
    } else {
        manage_child();
    }

    return EXIT_SUCCESS;
}
