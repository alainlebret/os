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

unsigned int h = 0; 
unsigned int m = 0; 
unsigned int s = 0;

void handle_sigint(int signal) {
    if (signal == SIGINT) {
        printf("Terminating...\n");
        exit(EXIT_SUCCESS);
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
        printf("%u:%u:%u (macOS, SIGALRM via setitimer)\n", h, m, s);
        fflush(stdout);
    }
}

int main(void) {
    struct sigaction action;
    struct sigaction sigint_action;
	
    /* Clean up the structure before using it */
    memset(&action, 0, sizeof(action));
    /* Set the new handler */
    action.sa_handler = &tick;
    action.sa_flags = SA_RESTART;
    /* Install the new handler of the SIGALRM signal */
    if (sigaction(SIGALRM, &action, NULL) == -1) {
        perror("sigaction SIGALRM");
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

    printf("Timer is armed (Mac OS X uses setitimer() and SIGALRM. Press Ctrl-C to quit.\n");

    /* Waiting for signals */
    while (1) {
        pause();
    }

    /* Unreachable: use <Ctrl-C> to exit */


    return EXIT_SUCCESS;
}
