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
#include <unistd.h>    /* fork() and sleep() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_03.c
 *
 * @brief This program demonstrates the use of fork() to create a child process
 * and uses sleep() to block both parent and child processes for a specified 
 * duration.
 */

#define DURATION 5

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
 * @brief Manages the parent process. 
 *
 * The parent process is blocked during \em DURATION seconds and waits for his
 * child.
 */
void manage_parent(pid_t child_pid) {
    int status;

    printf("Parent process (PID %d)\n", getpid());
    printf("Parent will be blocked during %d seconds...\n", DURATION);

    sleep(DURATION);

    printf("Parent has finished to sleep.\n");
    waitpid(child_pid, &status, 0);
    if (WIFEXITED(status)) {
        printf("Child process exited with status %d\n", WEXITSTATUS(status));
    }
}

/**
 * @brief Manages the child process. 
 *
 * The child process is blocked during \em DURATION seconds.
 */
void manage_child() {
    printf("Child process (PID %d)\n", getpid());
    printf("Child will be blocked during %d seconds...\n", DURATION);
    sleep(DURATION);
    printf("Child has finished to sleep.\n");
}

int main(void) {
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    }

    if (pid > 0) {
        manage_parent(pid);
    } else {
        manage_child();
    }

    return EXIT_SUCCESS;
}
