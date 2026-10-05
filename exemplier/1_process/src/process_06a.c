/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <unistd.h>    /* fork(), execl() and _exit() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_06a.c
 *
 * @brief Demonstrates process cloning using fork() and overlaying the child
 * with a new program using execl().
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
void manage_parent(void) {
    pid_t child;
    int status;

    printf("Parent process (PID %ld) waiting for the child.\n", (long) getpid());
    child = wait(&status);
    if (child == -1) {
        handle_fatal_error_and_exit("Error [wait()]");
    }
    if (WIFEXITED(status)) {
        printf("Parent (PID %ld): Child (PID %ld) finished with exit code: %d\n", (long) getpid(), (long) child, WEXITSTATUS(status));
    }
}

/**
 * @brief Manages the child process, replaces it with the 'ls' command.
 */
void manage_child(void) {
    const char *path = "/bin/ls";
    const char *command = "ls";
    const char *arguments = "-al";

    printf("Child process (PID %ld) will execute ls command.\n", (long) getpid());
    fflush(stdout); /* the stdio buffer would be lost by execl() */
    execl(path, command, arguments, (char *) NULL);

    /* If execl() returns, there was an error */
    perror("Error executing execl");
    _exit(127); /* not exit(): do not flush the stdio buffers inherited from the parent */
}

int main(void) {
    pid_t pid;
    
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Error [fork()]");
    }

    if (pid > 0) {
        manage_parent();
    } else {
        manage_child();
    }

    return EXIT_SUCCESS;
}
