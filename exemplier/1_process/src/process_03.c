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

#include <inttypes.h>  /* C99 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>     /* exit() */
#include <unistd.h>    /* fork() and sleep() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */

/**
 * @file process_03.c
 *
 * A simple program that clones a process using the \c fork() primitive and uses
 * sleep() to block parent and child processes.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.2
 * @date 2023-10-15
 */

#define DURATION 5

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
 * @brief Manages the parent process. 
 *
 * The parent process is blocked during \em DURATION seconds and waits for his
 * child.
 */
void manage_parent(pid_t child_pid)
{
	int status;
	
    printf("Parent process (PID %" PRId32 ")\n", getpid());
    printf("Parent will be blocked during %" PRId32 " seconds...\n", DURATION);

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
void manage_child()
{
    printf("Child process (PID %" PRId32 ")\n", getpid());
    printf("Child will be blocked during %" PRId32 " seconds...\n", DURATION);

    sleep(DURATION);

    printf("Child has finished to sleep.\n");
}

int main(void)
{
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
