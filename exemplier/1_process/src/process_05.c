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
 * @file process_05.c
 *
 * @brief A simple program that clones a process using the fork() primitive. 
 * The parent is waiting for his child to finish.
 */

#define DURATION 20

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
 * The parent is waiting for his child to exit.
 */
void manage_parent() {
    pid_t child;
    int status;

    printf("Parent process (PID %d) is waiting for the child to finish.\n", getpid());
    child = wait(&status);
    if (WIFEXITED(status)) {
        printf("Parent (PID %d): Child (PID %d) has finished with exit code: %d\n",
               getpid(), child, WEXITSTATUS(status));
    }
}

/**
 * @brief Manages the child process, simulating work by sleeping.
 */
void manage_child() {
    printf("Child process (PID %d) starts and will be blocked for %d seconds.\n", getpid(), DURATION);
    sleep(DURATION);
    printf("Child (PID %d) has finished sleeping. My parent's PID is %d.\n", getpid(), getppid());
}

int main(void) {
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Error [fork()]: ");
    }

    if (pid > 0) {
        manage_parent();
    } else {
        manage_child();
    }

    return EXIT_SUCCESS;
}
