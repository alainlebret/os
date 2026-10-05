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
 * @file linux_clock.c
 *
 * A simple program that uses POSIX timers and handles SIGRTMIN to create a clock.
 * Linux only: macOS has neither timer_create() nor SIGRTMIN (see macosx_clock.c).
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

volatile sig_atomic_t h = 0; /* Hours */
volatile sig_atomic_t m = 0; /* Minutes */
volatile sig_atomic_t s = 0; /* Seconds */
volatile sig_atomic_t ticked = 0; /* set by tick() */
volatile sig_atomic_t stop = 0;   /* set by handle_sigint() */

/** 
 * @brief Signal handler for SIGINT signal.
 * @param signal Number of the signal
 */
void handle_sigint(int signal) 
{
    /* Only a flag: printf() and exit() are not async-signal-safe */
    if (signal == SIGINT) {
        stop = 1;
    }
}

/** 
 * @brief Signal handler for real-time signal.
 * @param signal Number of the signal
 */
void tick(int signal) 
{
    if (signal == SIGRTMIN) {
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
        /* No need to re-arm manually */
    }
}

int main(void) 
{
    struct sigaction action;
    struct sigaction sigint_action;
    sigset_t mask, old_mask;

    /* Initialize the structure to zero before use. */
    memset(&action, 0, sizeof(action));
    /* Set the new handler */
    action.sa_handler = &tick;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    /* Install the new handler of the SIGRTMIN signal */
    if (sigaction(SIGRTMIN, &action, NULL) == -1) {
        perror("sigaction SIGRTMIN");
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

    /* Block SIGRTMIN and SIGINT: they are only received inside sigsuspend() */
    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);
    sigaddset(&mask, SIGINT);
    if (sigprocmask(SIG_BLOCK, &mask, &old_mask) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    /* Create the POSIX timer */
    struct sigevent sev;
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;

    timer_t timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    /* Arm the timer for each second */
    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    printf("Timer is armed (Linux, SIGRTMIN). Press Ctrl-C to quit.\n");

    /* Waiting for signals (blocked outside sigsuspend(): no lost wake-up) */
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
