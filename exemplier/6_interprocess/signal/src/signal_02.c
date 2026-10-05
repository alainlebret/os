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
#include <unistd.h>    /* alarm(), write(), _exit() */
#include <signal.h>    /* sigaction() */
#include <string.h>    /* memset() */

/**
 * @file signal_02.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM.
 */

#define DURATION 5

/**
 * @brief Signal handler for SIGALRM signal.
 *
 * Only async-signal-safe functions: write() and _exit() (printf() and
 * exit() are forbidden in a handler).
 * @param signal Number of the signal
 */
void handle_alarm(int signal) {
    const char msg[] = "\nToo late!\n";

    if (signal == SIGALRM) {
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        _exit(EXIT_FAILURE);
    }
}

int main(void) {
    struct sigaction action;
    int value;
    int remaining_time;
    int result;

    /* Initialize the structure to zero before use. */
    memset(&action, '\0', sizeof(action));

    /* Set the new handler */
    action.sa_handler = &handle_alarm;
    sigemptyset(&action.sa_mask);

    /* Install the new handler of the SIGALRM signal */
    if (sigaction(SIGALRM, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("You have %d seconds to enter a number: ", DURATION);
    fflush(stdout); /* _exit() in the handler does not flush stdio buffers */
    /* The OS will send an alarm signal to the process in 'DURATION' sec. */
    alarm(DURATION);

    /* Wait for the user to enter a value */
    result = scanf("%d", &value);
    if (result < 1) {
        printf("Failed to read a valid number.\n");
        return EXIT_FAILURE;
    }

    /* Deactivate the sending of an alarm signal by the OS */
    remaining_time = alarm(0);
    printf("\nYou entered the number %d with %d seconds remaining.\n", value, remaining_time);

    return EXIT_SUCCESS;
}
