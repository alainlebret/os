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
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_06b.c
 *
 * @brief Demonstrates cloning a process with fork() and replacing the
 * child with gnuplot using execlp().
 */

/**
 * @brief Handles a fatal error and exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process by waiting for the child to exit.
 */
void manage_parent() {
    pid_t child;
    int status;

    printf("Parent process (PID %d) waiting for the child.\n", getpid());
    child = wait(&status);
    if (WIFEXITED(status)) {
        printf("Parent (PID %d): Child (PID %d) finished with exit code: %d\n", getpid(), child, WEXITSTATUS(status));
    }
}

/**
 * @brief Manages the child process, replaces it with the gnuplot command.
 */
void manage_child() {
    const char *path = "gnuplot";
    const char *command = "gnuplot";
    const char *argument1 = "-persist";
    const char *argument2 = "resources/command.gp";

    printf("Child process (PID %d) will execute Gnuplot.\n", getpid());
    if (execlp(path, command, argument1, argument2, NULL) == -1) {
        handle_fatal_error_and_exit("Failed to run Gnuplot using execlp()");
    }
}

int main(void) {
    pid_t pid = fork();
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
