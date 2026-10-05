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
#include <stdio.h>     /* printf(), perror() */
#include <stdlib.h>    /* EXIT_SUCCESS, EXIT_FAILURE */
#include <unistd.h>    /* getpid() */
#include <signal.h>    /* sigaction(), sigprocmask(), sigsuspend() */

/**
 * @file signal_sigsuspend.c
 * @brief Waits for SIGUSR1 without race condition using sigsuspend().
 *
 * Course "Operating Systems", chapter « Signaux », slides
 * « Communiquer avec le programme principal », « Erreur classique : tester
 * puis pause() » and « Attendre sans situation de compétition : sigsuspend() ».
 *
 * The handler only sets a volatile sig_atomic_t flag. With the naive loop
 * "while (!recu) pause();", a signal arriving between the test and pause()
 * would be lost. Here SIGUSR1 is blocked during the test (it stays pending)
 * and sigsuspend() atomically restores the old mask and waits.
 *
 * \code{.bash}
 *   $ ./signal_sigsuspend &
 *   PID 12345: waiting for SIGUSR1 (kill -USR1 12345)
 *   $ kill -USR1 12345
 *   SIGUSR1 received, normal flow resumed.
 * \endcode
 */

static volatile sig_atomic_t recu = 0;

/**
 * @brief Handler of SIGUSR1: only sets the flag (async-signal safe).
 * @param sig Number of the received signal.
 */
static void gestionnaire(int sig) {
    (void) sig;
    recu = 1;          /* safe operation */
}

int main(void) {
    struct sigaction sa;
    sigset_t masque, ancien;

    /* Install the handler of SIGUSR1 */
    sa.sa_handler = gestionnaire;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }

    /* Block SIGUSR1 before testing the flag */
    sigemptyset(&masque);
    sigaddset(&masque, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &masque, &ancien) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    printf("PID %ld: waiting for SIGUSR1 (kill -USR1 %ld)\n",
           (long) getpid(), (long) getpid());
    fflush(stdout);

    while (!recu) {
        sigsuspend(&ancien);  /* unblocks and waits, atomically */
    }

    /* Restore the previous mask */
    sigprocmask(SIG_SETMASK, &ancien, NULL);

    printf("SIGUSR1 received, normal flow resumed.\n");

    return EXIT_SUCCESS;
}
