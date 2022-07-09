/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file signal_03.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM.
 */

#include <stdio.h>     /* printf() */
#include <unistd.h>    /* fork() */
#include <signal.h>    /* sigaction() */
#include <string.h>    /* memset() */

#define FOREVER for (;;)

unsigned int h = 0; /* Hours */
unsigned int m = 0; /* Minutes */
unsigned int s = 0; /* Seconds */

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
        printf("%d:%d:%d\n", h, m, s);

        /* Re-engage the alarm */
        alarm(1);
    }
}

int main(void)
{
    struct sigaction action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));

    /* Set the new handler */
    action.sa_handler = &tick;

    /* Install the new handler of the SIGALRM signal */
    sigaction(SIGALRM, &action, NULL);

    /* Ask the OS to send a SIGALRM signal in 1 second */
    alarm(1);

    /* Waiting for SIGALRM signal */
    FOREVER {}

    /* Unreachable: use <Ctrl-C> to exit */

}
