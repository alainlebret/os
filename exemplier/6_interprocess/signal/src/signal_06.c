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
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork() and pause() */
#include <sys/types.h> /* pid_t */
#include <signal.h>    /* sigaction */

/**
 * @file signal_06.c
 *
 * A simple program that uses POSIX signals and manages masks to block signals.
 */

#define NBR_SIGNALS 2

/** A group of signals to be blocked by the process */
int signals[NBR_SIGNALS] = {SIGINT, SIGTERM};

/** An array to store the old signal handlers */
struct sigaction old_handlers[NBR_SIGNALS];

int main(void) {
    int i;
    sigset_t new_mask;
    sigset_t old_mask;
    sigset_t pending_signals;

    /*
     * Create a mask to block the two signals.
     */
    sigemptyset(&new_mask);
    for (i = 0; i < NBR_SIGNALS; i++) {
        sigaddset(&new_mask, signals[i]);
    }

    /*
     * Exchange old and new masks
     */
    sigprocmask(SIG_SETMASK, &new_mask, &old_mask);

    /*
     * Sleep for 20 seconds (maybe enough to try sending CTRL-C and SIGTERM and
     * SIGHUP signals)...
     */
    printf("20 seconds to send <CTRL>-C and kill -15 %d to this process\n", getpid());
    sleep(20);

    /* Get the list of pending signals */
    sigpending(&pending_signals);

    /* Decode pending signals */
    for (i = 1; i < NSIG; i++) {
        if (sigismember(&pending_signals, i)) {
            printf("Pending signal %d has been blocked.\n", i);
        }
    }

    /* Unblocking signals */
    sigprocmask(SIG_SETMASK, &old_mask, NULL);
    printf("\nSignals unblocked. Normal operation resumed.\n");

    return EXIT_SUCCESS;
}
