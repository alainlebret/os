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

/**
 * @file macosx_clock.c
 *
 * A simple program that uses timer and handles SIGALRM to create a clock.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

volatile sig_atomic_t h = 0;
volatile sig_atomic_t m = 0;
volatile sig_atomic_t s = 0;
volatile sig_atomic_t ticked = 0; /* set by tick() */
volatile sig_atomic_t stop = 0;   /* set by handle_sigint() */

void handle_sigint(int signal) {
    /* Only a flag: printf() and exit() are not async-signal-safe */
    if (signal == SIGINT) {
        stop = 1;
    }
}

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
        ticked = 1;   /* the time is displayed by main() */
    }
}

int main(void) {
    struct sigaction action;
    struct sigaction sigint_action;
    sigset_t mask, old_mask;
	
    /* Initialize the structure to zero before use. */
    memset(&action, 0, sizeof(action));
    /* Set the new handler */
    action.sa_handler = &tick;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    /* Install the new handler of the SIGALRM signal */
    if (sigaction(SIGALRM, &action, NULL) == -1) {
        perror("sigaction SIGALRM");
        exit(EXIT_FAILURE);
    }

    /* Repeat the same setup for SIGINT. */
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = &handle_sigint;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }

    /* Block SIGALRM and SIGINT: they are only received inside sigsuspend() */
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, &old_mask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    /* On Mac OS X, POSIX timers are not available, we must use setitimer() */
    struct itimerval itv;
    itv.it_value.tv_sec = 1;
    itv.it_value.tv_usec = 0;
    itv.it_interval.tv_sec = 1;
    itv.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }

    printf("Timer is armed (Mac OS X uses setitimer() and SIGALRM). Press Ctrl-C to quit.\n");

    /* Waiting for signals (blocked outside sigsuspend(): no lost wake-up) */
    while (!stop) {
        sigsuspend(&old_mask);
        if (ticked) {
            ticked = 0;
            printf("%d:%d:%d (macOS, SIGALRM via setitimer)\n", (int) h, (int) m, (int) s);
        }
    }
    sigprocmask(SIG_SETMASK, &old_mask, NULL);
    printf("Terminating...\n");

    return EXIT_SUCCESS;
}
