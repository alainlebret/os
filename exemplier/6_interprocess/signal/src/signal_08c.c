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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

/**
 * @file signal_08c.c
 *
 * A simple program to handle SIGUSR1 and SIGUSR2 signals with two distinct
 * handlers, but this time we mask other signals.
 * Open another terminal while running this program and execute:
 * kill -s SIGUSR1 <PID>
 * What if using the following command?
 * kill -s SIGINT <PID>
 * To stop the program: kill -s SIGKILL <PID>
 */

/**
 * @brief Handles SIGUSR1 (write() is async-signal-safe, printf() is not).
 */
void do_when_sigusr1(int sig) {
    const char msg[] = "Have received one SIGUSR1\n";

    (void) sig;
    int saved_errno = errno; /* write() may change errno: restore it for main() */
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    errno = saved_errno;
}

/**
 * @brief Handles SIGUSR2 (write() is async-signal-safe, printf() is not).
 */
void do_when_sigusr2(int sig) {
    const char msg[] = "Have received one SIGUSR2\n";

    (void) sig;
    int saved_errno = errno; /* write() may change errno: restore it for main() */
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    errno = saved_errno;
}

int main(void) {
    int is_ok;
    sigset_t set;
    sigset_t oset;
    struct sigaction action;

    /* Add all signals to the mask (SIGKILL and SIGSTOP cannot be blocked) */
    sigfillset(&set);
    /* Remove SIGUSR1 and SIGUSR2 from the mask */
    sigdelset(&set, SIGUSR1);
    sigdelset(&set, SIGUSR2);
    /* Install the mask to receive only SIGUSR1 and SIGUSR2 */
    is_ok = sigprocmask(SIG_SETMASK, &set, &oset);

    if (is_ok == -1) {
        perror("PB");
    }
    if (sigismember(&set, SIGUSR1) || sigismember(&set, SIGUSR2)) {
        printf("Problem with the mask\n");
        exit(EXIT_FAILURE);
    }
    if (!sigismember(&oset, SIGUSR1) && !sigismember(&oset, SIGUSR2)) {
        printf("The old mask was not blocking signals\n");
    }
    printf("PID %ld: only SIGUSR1 or SIGUSR2 may be received\n", (long) getpid());

    /* Initialize the structure to zero before use. */
    memset(&action, '\0', sizeof(action));

    action.sa_mask = set;
    action.sa_handler = do_when_sigusr1;
    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    action.sa_handler = do_when_sigusr2;
    if (sigaction(SIGUSR2, &action, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sleep(1);
    }

    return 0;
}
