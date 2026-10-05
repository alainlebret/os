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
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>     /* perror() */
#include <stdlib.h>    /* exit() */
#include <string.h>    /* memset() */
#include <unistd.h>    /* write(), pause() */
#include <errno.h>
#include <signal.h>    /* sigaction(), SIGRTMIN, struct sigevent */
#include <time.h>      /* timer_create(), timer_settime() */

/**
 * @file posix_timer_tic.c
 * @brief A periodic POSIX timer that prints "tic" (Linux only).
 *
 * Course "Operating Systems", chapter « Signaux », part « Minuteries POSIX »,
 * slides « Exemple (1/3) » to « Exemple (3/3) ».
 *
 * The timer uses CLOCK_MONOTONIC and notifies the process with SIGRTMIN.
 * The default action of SIGRTMIN terminates the process: a handler is
 * therefore mandatory. First expiration after 1.02 s, then every 500 ms.
 * Stop with Ctrl-C.
 *
 * timer_create() and SIGRTMIN do not exist on macOS: see macosx_clock.c
 * (setitimer()). With glibc < 2.34, link with -lrt.
 *
 * \code{.bash}
 *   $ ./posix_timer_tic
 *   tic
 *   tic
 *   ^C
 * \endcode
 */

/**
 * @brief Handler of SIGRTMIN: write() is async-signal safe, printf() is not.
 * @param sig Number of the received signal.
 */
static void gestionnaire(int sig) {
    (void) sig;
    const char msg[] = "tic\n";
    int saved_errno = errno; /* write() may change errno: restore it for main() */
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    errno = saved_errno;
}

int main(void) {
    struct sigaction sa;
    struct sigevent sev;
    struct itimerspec its;
    timer_t timerid;

    /* (1/3) Install the handler */
    sa.sa_handler = gestionnaire;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* (2/3) Create the timer */
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;  /* chosen signal */

    if (timer_create(CLOCK_MONOTONIC, &sev,
                     &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    /* (3/3) Arm the timer */
    /* first expiration after 1 s and 20 ms */
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 20000000L;
    /* then every 500 ms */
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 500000000L;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }
    for (;;)
        pause();

    /* Unreachable: use <Ctrl-C> to exit (timer_delete(timerid) otherwise) */
}
