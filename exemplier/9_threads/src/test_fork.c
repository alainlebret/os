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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> /* necessary for wait */
#include <sys/wait.h> /* necessary for wait */

/**
 * @file test_fork.c
 *
 * A simple program to test fork vs threads (see \c test_pthread.c).
 * The elapsed (wall-clock) time is measured with clock_gettime(): clock()
 * would only count the CPU time of the parent, not that of its children.
 *
 * Compile using gcc -Wall -Wextra test_fork.c -o test_fork
 */

#define NB_FORKS 50000

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void do_little(void) {
    int i;

    i = 0;
    i = i + 2;

    exit(EXIT_SUCCESS);
}

int main(void) {
    pid_t pid;
    int status;
    int i;
    struct timespec begin_time;
    struct timespec end_time;
    double duration;

    status = 0;

    clock_gettime(CLOCK_MONOTONIC, &begin_time);

    for (i = 0; i < NB_FORKS; i++) {
        if ((pid = fork()) == -1) {
            handle_fatal_error("Error when trying to fork");
        } else if (pid == 0) {
            do_little();
        } else {
            waitpid(pid, &status, 0);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    duration = (end_time.tv_sec - begin_time.tv_sec)
               + (end_time.tv_nsec - begin_time.tv_nsec) / 1e9;
    printf("%2.1f seconds\n", duration);

    return EXIT_SUCCESS;
}  
