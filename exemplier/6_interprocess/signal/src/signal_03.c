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
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork() */
#include <signal.h>    /* sigaction() */
#include <string.h>    /* memset() */

/**
 * @file signal_03.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM to create a clock.
 *
 * @author Alain Lebret
 * @version	1.0.1
 * @date 2023-12-01
 */

#define FOREVER for (;;)

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
 * @brief Defines the new handler of the SIGALRM signal.
 * @param signal Number of the signal
 */
void tick(int signal)
{
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
        printf("%u:%u:%u\n", h, m, s);

        /* Re-engage the alarm */
        alarm(1);
    }
}

int main(void)
{
    struct sigaction action;
    struct sigaction sigint_action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));
    /* Set the new handler */
    action.sa_handler = &tick;
    /* Install the new handler of the SIGALRM signal */
    sigaction(SIGALRM, &action, NULL);

	/* Same for SIGINT */
    memset(&sigint_action, '\0', sizeof(sigint_action));
    sigint_action.sa_handler = &handle_sigint;
    sigaction(SIGINT, &sigint_action, NULL);

    /* Ask the OS to send a SIGALRM signal in 1 second */
    alarm(1);

    /* Waiting for SIGALRM signal */
    while (1) {
        pause();
    }

    /* Unreachable: use <Ctrl-C> to exit */

}
