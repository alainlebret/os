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
#include <signal.h>    /* sigaction(), sigprocmask(), sigsuspend() */
#include <string.h>    /* memset() */

/**
 * @file signal_03.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM to create a clock.
 */

volatile sig_atomic_t h = 0; /* Hours */
volatile sig_atomic_t m = 0; /* Minutes */
volatile sig_atomic_t s = 0; /* Seconds */
volatile sig_atomic_t ticked = 0;   /* set by tick() */
volatile sig_atomic_t stop = 0;     /* set by handle_sigint() */

/** 
 * @brief Signal handler for SIGINT signal.
 *
 * Only sets a flag: printf() and exit() are not async-signal-safe.
 * @param signal Number of the signal
 */
void handle_sigint(int signal) {
    if (signal == SIGINT) {
        stop = 1;
    }
}

/** 
 * @brief Signal handler for SIGALRM signal.
 *
 * Updates the clock and re-engages the alarm (alarm() is async-signal-safe).
 * The time is displayed by main(), not here.
 * @param signal Number of the signal
 */
void tick(int signal) {
    if (signal == SIGALRM) {
        s++;
        if (s == 60) {
            s = 0;
            m++;
            if (m == 60) {
                m = 0;
                h++;
                if (h == 24)
                    h = 0;
            }
        }
        ticked = 1;

        /* Re-engage the alarm */
        alarm(1);
    }
}

int main(void) {
    struct sigaction action;
    struct sigaction sigint_action;
    sigset_t mask, old_mask;

    /* Initialize the structure to zero before use. */
    memset(&action, '\0', sizeof(action));
    /* Set the new handler */
    action.sa_handler = &tick;
    sigemptyset(&action.sa_mask);
    /* Install the new handler of the SIGALRM signal */
    if (sigaction(SIGALRM, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Repeat the same setup for SIGINT. */
    memset(&sigint_action, '\0', sizeof(sigint_action));
    sigint_action.sa_handler = &handle_sigint;
    sigemptyset(&sigint_action.sa_mask);
    if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Block SIGALRM and SIGINT outside sigsuspend() (no lost wake-up) */
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, &old_mask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    /* Ask the OS to send a SIGALRM signal in 1 second */
    alarm(1);

    /* Waiting for signals: use <Ctrl-C> to exit */
    while (!stop) {
        sigsuspend(&old_mask);
        if (ticked) {
            ticked = 0;
            printf("%d:%d:%d\n", (int) h, (int) m, (int) s);
        }
    }
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    printf("Terminating...\n");

    return EXIT_SUCCESS;
}
