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
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */
#include <signal.h>    /* sigaction */
#include <string.h>    /* memset() */
#include <errno.h>     /* errno */

/**
 * @file signal_04.c
 *
 * A simple program that uses POSIX signals and handles SIGCHLD.
 */

static volatile sig_atomic_t child_exited = 0;
static volatile sig_atomic_t last_child = 0;   /* PID of the last reaped child */

/**
 * @brief Defines a new handler of the SIGCHLD signal in charge of suppressing
 * zombies.
 *
 * Only async-signal-safe operations are used here: waitpid() and assignments
 * to sig_atomic_t variables. The message is printed later, in the normal
 * flow of the program (printf() must never be called from a handler).
 * @param signal Number of the signal.
 */
void handle_sigchild(int signal) {
    pid_t child;
    int saved_errno = errno;   /* waitpid() may modify errno */

    if (signal == SIGCHLD) {
        while ((child = waitpid(-1, NULL, WNOHANG)) > 0) {
            last_child = (sig_atomic_t)child;
        }
        child_exited = 1;
    }
    errno = saved_errno;
}

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
 * @brief Installs the SIGCHLD handler. Called before fork(): the handler is
 * then in place even if the child ends before the parent runs again.
 */
void install_sigchld_handler(void) {
    struct sigaction action;

    /* Initialize the structure to zero before use. */
    memset(&action, '\0', sizeof(action));
    /* Set the new handler */
    action.sa_handler = &handle_sigchild;
    sigemptyset(&action.sa_mask);
    /* We ensure that certain system calls are automatically restarted if
     * interrupted by a signal; no SIGCHLD when a child is only stopped */
    action.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    /* Install the new handler of the SIGCHLD signal */
    if (sigaction(SIGCHLD, &action, NULL) == -1) {
        handle_fatal_error_and_exit("sigaction");
    }
}

/**
 * @brief Manages the parent process.
 */
void manage_parent(void) {
    printf("Parent process (PID %ld)\n", (long) getpid());

    while (!child_exited) {
        printf("Parent: I am working...\n");
        sleep(2);
    }

    printf("My child (%ld) died. He will not be a zombie.\n", (long) last_child);
    printf("Parent: My child has exited, so I can stop working :-)\n");
}

/**
 * @brief Manages the child process. 
 *
 * The child simulates work for 10 seconds.
 */
void manage_child(void) {
    printf("Child process (PID %ld)\n", (long) getpid());
    printf("Child: I am doing some stuff for 10 seconds...\n");
    sleep(10);
    exit(EXIT_SUCCESS);
}

int main(void) {
    pid_t pid;

    install_sigchld_handler(); /* before fork() */
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
