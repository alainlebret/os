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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> /* necessary for wait */
#include <sys/wait.h> /* necessary for wait */

/**
 * @file fork_test.c
 *
 * A simple program to test fork vs threads (see \c thread_test.c).
 *
 * Compile using gcc -Wall -Wextra fork_test.c
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
    int pid;
    int status;
    int i;
    clock_t begin_time;
    clock_t end_time;
    double duration;

    status = 0;

    begin_time = clock();

    for (i = 0; i < NB_FORKS; i++) {
        if ((pid = fork()) < 0) {
            handle_fatal_error("Error when trying to fork\n");
        } else if (pid == 0) {
            do_little();
        } else {
            waitpid(pid, &status, 0);
        }
    }

    end_time = clock();
    duration = (double) (end_time - begin_time) / CLOCKS_PER_SEC;
    printf("%2.1f seconds\n", duration);

    return EXIT_SUCCESS;
}  
