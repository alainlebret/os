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
#include <signal.h>    /* sigaction */
#include <string.h>    /* memset() */

/**
 * @file signal_04.c
 *
 * A simple program that uses POSIX signals and handles SIGCHLD.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2011-12-01
 */

#define FOREVER for (;;)

int child_exited = 0;

/** 
 * @brief Defines a new handler of the SIGCHLD signal in charge of suppressing
 * zombies.
 * @param signal Number of the signal.
 */
void handle_sigchild(int signal)
{
    pid_t child;
    int status;

    if (signal == SIGCHLD) {
        while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("My child (%d) died. He will not be a zombie.\n", child);
        }
        child_exited = 1;
    }
}

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
 */
void manage_parent()
{
    struct sigaction action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));
    /* Set the new handler */
    action.sa_handler = &handle_sigchild;
	/* We ensure that certain system calls are automatically restarted if interrupted by a signal */
    action.sa_flags = SA_RESTART;

    /* Install the new handler of the SIGCHLD signal */
    sigaction(SIGCHLD, &action, NULL);

    printf("Parent process (PID %d)\n", getpid());

    while (!child_exited) {
        printf("Parent: I am working...\n");
        sleep(2);
    }

    printf("Parent: My child has exited, so I can stop working :-)\n");
}

/**
 * @brief Manages the child process. 
 *
 * It just does some stuff for 10 seconds.
 */
void manage_child()
{
    printf("Child process (PID %d)\n", getpid());
    printf("Child: I am doing some stuff for 10 seconds...\n");
    sleep(10);
    exit(EXIT_SUCCESS);
}

int main(void)
{
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
