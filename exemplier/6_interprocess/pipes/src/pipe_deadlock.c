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
#include <stdio.h>     /* perror(), printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* pipe(), fork(), read(), write(), alarm() */
#include <signal.h>    /* sigaction(), kill() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid() */

/**
 * @file pipe_deadlock.c
 * @brief Deadlock between a parent and its child using two pipes.
 *
 * Course "Operating Systems", chapter « Tubes », slides
 * « Interblocage (deadlock) » and « Interblocage : attente circulaire ».
 *
 * t1 carries data from the parent to the child, t2 from the child to the
 * parent. Each process first waits for the other one (read), and nobody
 * writes as long as it has not read: circular wait, mutual blocking.
 *
 * Addition to the slide: an alarm of DELAY seconds lets the parent detect
 * the deadlock, kill the child and stop, instead of blocking forever.
 * Fix: a request/response protocol stating who speaks first (e.g. the
 * parent writes, then reads the response).
 *
 * \code{.bash}
 *   $ ./pipe_deadlock
 *   Parent and child are both waiting for each other...
 *   Deadlock: no data after 3 s, the parent kills its child.
 * \endcode
 */

#define DELAY 3

/* Written by main() before alarm() starts the timer, so before the handler can
   run: the handler only reads it (a pid_t is not a sig_atomic_t) */
static volatile pid_t child_pid = 0;

/**
 * @brief Handler of SIGALRM: only async-signal-safe calls (write, kill, _exit).
 * @param sig Number of the received signal.
 */
static void handle_alarm(int sig) {
    (void) sig;
    /* DELAY is a macro: the string is built at compile time (no printf() here) */
#define STR2(x) #x
#define STR(x) STR2(x)
    const char msg[] = "Deadlock: no data after " STR(DELAY) " s, the parent kills its child.\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
    if (child_pid > 0)
        kill(child_pid, SIGKILL);
    _exit(EXIT_FAILURE);
}

int main(void) {
    int t1[2], t2[2];   /* t1: parent -> child */
    char buf[64];       /* t2: child -> parent */
    struct sigaction sa;
    pid_t pid;

    if (pipe(t1) == -1 || pipe(t2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        close(t1[1]);                  /* unused ends */
        close(t2[0]);
        read(t1[0], buf, sizeof(buf)); /* waits */
        write(t2[1], "reponse", 7);
        exit(EXIT_SUCCESS);
    }

    close(t1[0]);                      /* unused ends */
    close(t2[1]);

    /* Addition: watchdog to end the demonstration */
    child_pid = pid;
    sa.sa_handler = handle_alarm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    alarm(DELAY);

    printf("Parent and child are both waiting for each other...\n");
    fflush(stdout);

    read(t2[0], buf, sizeof(buf));     /* waits */
    write(t1[1], "requete", 7);

    /* Never reached */
    waitpid(pid, NULL, 0);
    return EXIT_SUCCESS;
}
