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

/**
 * @file linux_clock.c
 *
 * A simple program that uses POSIX timers and handles SIGRTMIN to create a clock.
 */

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

unsigned int h = 0; /* Hours */
unsigned int m = 0; /* Minutes */
unsigned int s = 0; /* Seconds */

/** 
 * @brief Defines the new handler of the SIGINT signal.
 * @param signal Number of the signal
 */
void handle_sigint(int signal) 
{
    if (signal == SIGINT) {
        printf("Terminating...\n");
        exit(EXIT_SUCCESS);
    }
}

/** 
 * @brief Defines the new handler of the real-time signal.
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
        printf("%u:%u:%u\n", h, m, s);
        fflush(stdout);
        /* No need to re-arm manually */
    }
}

int main(void) 
{
    struct sigaction action;
    struct sigaction sigint_action;

    /* Clean up the structure before using it */
    memset(&action, 0, sizeof(action));
    /* Set the new handler */
    action.sa_handler = &tick;
    action.sa_flags = SA_RESTART;
    /* Install the new handler of the SIGALRM signal */
    if (sigaction(SIGRTMIN, &action, NULL) == -1) {
        perror("sigaction SIGRTMIN");
        exit(EXIT_FAILURE);
    }

    /* Same for SIGINT */
    memset(&sigint_action, 0, sizeof(sigint_action));
    sigint_action.sa_handler = &handle_sigint;
    sigint_action.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sigint_action, NULL) == -1) {
        perror("sigaction SIGINT");
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

    /* Waiting for SIGRTMIN signal */
    while (1) {
        pause();
    }

    /* Unreachable: use <Ctrl-C> to exit */

    return EXIT_SUCCESS;
}
