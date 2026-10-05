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
#include <unistd.h>    /* fork(), pause(), sleep() */
#include <signal.h>    /* kill(), SIGUSR1 */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid(), WIFSIGNALED() */

/**
 * @file signal_kill_child.c
 * @brief The parent sends SIGUSR1 to its child with kill().
 *
 * Course "Operating Systems", chapter « Signaux », slide
 * « Exemple parent -> enfant ».
 *
 * The child installs no handler: the default action of SIGUSR1 terminates
 * it. The parent then decodes the status returned by waitpid().
 *
 * Note: without the test pid == -1, a failed fork() would lead to
 * kill(-1, SIGUSR1), which sends the signal to ALL your processes.
 *
 * \code{.bash}
 *   $ ./signal_kill_child
 *   Parent: sending SIGUSR1 to the child ...
 *   Parent: child ... killed by signal ... (SIGUSR1)
 * \endcode
 */

int main(void) {
    int status;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }
    if (pid == 0) {
        pause();     /* child: killed by SIGUSR1 */
        return EXIT_SUCCESS;
    }
    sleep(1);        /* parent: sends SIGUSR1 */
    printf("Parent: sending SIGUSR1 to the child %ld\n", (long) pid);
    if (kill(pid, SIGUSR1) == -1) {
        perror("kill");
        return EXIT_FAILURE;
    }
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFSIGNALED(status)) {
        printf("Parent: child %ld killed by signal %d%s\n", (long) pid,
               WTERMSIG(status),
               WTERMSIG(status) == SIGUSR1 ? " (SIGUSR1)" : "");
    } else if (WIFEXITED(status)) {
        printf("Parent: child %ld exited with code %d\n", (long) pid,
               WEXITSTATUS(status));
    }

    return EXIT_SUCCESS;
}
