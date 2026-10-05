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

#define _POSIX_C_SOURCE 200809L /* kill(), setpgid() with -std=c11 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* getpid(), getpgrp(), setpgid() and pause() */
#include <signal.h>    /* kill() and SIGTERM */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait(), WIFEXITED and WEXITSTATUS */

/**
 * @file process_07.c
 *
 * @brief A simple program about a process and its group.
 *
 * The child leaves the group of its parent with setpgid(0, 0): it becomes the
 * leader of a new group whose PGID is its PID. The parent makes the same call
 * (setpgid(pid, pid)), so that the group exists whoever runs first, then sends
 * SIGTERM to the whole group with kill(-pgid, SIGTERM) (killpg(pgid, SIGTERM)
 * does the same, but is an XSI extension).
 *
 * Course "Operating Systems", chapter « Signaux », slide « Groupes de processus ».
 */

/**
 * @brief Handles a fatal error and exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process: displays its group, sends SIGTERM to
 * the group of the child, then waits for it.
 */
void manage_parent(pid_t pid) {
    pid_t child;
    int status;

    printf("Parent process: PID=%ld, Group ID=%ld\n", (long) getpid(), (long) getpgrp());

    /* Same call as in the child: no race on who runs first.
       EACCES: the child has already called exec (not the case here). */
    if (setpgid(pid, pid) == -1) {
        perror("setpgid (parent)");
    }
    sleep(1); /* let the child display its new group */

    printf("Parent: SIGTERM to the group %ld\n", (long) pid);
    if (kill(-pid, SIGTERM) == -1) { /* -pid: the whole group of PGID pid */
        handle_fatal_error_and_exit("kill");
    }

    child = wait(&status);
    if (child == -1) {
        handle_fatal_error_and_exit("wait");
    }

    if (WIFEXITED(status)) {
        printf("Parent (PID %ld): Child (PID %ld) exited with code %d\n",
                (long) getpid(), (long) child, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Parent (PID %ld): Child (PID %ld) killed by signal %d\n",
                (long) getpid(), (long) child, WTERMSIG(status));
    }
}

/**
 * @brief Manages the child process: creates its own group, then waits.
 */
void manage_child(void) {
    printf("Child process: PID=%ld, Group ID=%ld\n", (long) getpid(), (long) getpgrp());

    /* New group, led by the child: PGID = PID of the child */
    if (setpgid(0, 0) == -1) {
        handle_fatal_error_and_exit("setpgid (child)");
    }
    printf("Child process: PID=%ld, new Group ID=%ld\n", (long) getpid(), (long) getpgrp());

    fflush(stdout); /* killed by SIGTERM: an unflushed stdio buffer would be lost */
    pause();        /* waits for the SIGTERM of its parent */
}

int main(void) {
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Fork failed: unable to create child process");
    }

    if (pid > 0) {
        manage_parent(pid);
    } else {
        manage_child();
    }

    return EXIT_SUCCESS;
}