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
#include <stdio.h>    /* printf() */
#include <stdlib.h>   /* exit() */
#include <string.h>   /* memset() */
#include <signal.h>   /* sigaction(), sigprocmask(), sigsuspend() */
#include <unistd.h>

/**
 * @file signal_05.c
 *
 * A simple program that uses POSIX signals and handles many signals.
 */

volatile sig_atomic_t exit_flag = 0;
volatile sig_atomic_t received = 0; /* number of the last received signal */

/**
 * @brief Records the received signal number; main() displays it.
 *
 * printf() is not async-signal-safe: the handler only sets flags.
 * @param signal Number of the signal.
 */
void handle(int signal) {
    received = signal;

    if (signal == SIGINT || signal == SIGTERM) {
        exit_flag = 1;
    }
}

int main(void) {
    struct sigaction action;
    sigset_t mask, old_mask;

    /* Initialize the structure to zero before use. */
    memset(&action, '\0', sizeof(action));
    /* Set the new handler */
    action.sa_handler = &handle;
    /* We do not block any specific signal */
    sigemptyset(&action.sa_mask);

    /* Three signals will be handled by the process */
    if (sigaction(SIGINT, &action, 0) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGQUIT, &action, 0) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &action, 0) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Block the three signals outside sigsuspend() (no lost wake-up) */
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &mask, &old_mask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    while (!exit_flag) {
        sigsuspend(&old_mask); /* Wait for a signal */
        printf("Signal number %d has been received.\n", (int) received);
    }
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    printf("Exiting more gracefully.\n");

    return EXIT_SUCCESS;
}
