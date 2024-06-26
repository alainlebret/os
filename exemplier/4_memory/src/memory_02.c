/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork(), sleep() */
#include <sys/types.h> /* pid_t */
#include <time.h>      /* time() */
#include <signal.h>    /* signal handling */

/**
 * @file memory_02.c
 *
 * This program runs for approximately 4 minutes, during which its memory 
 * mapping can be inspected using system tools (e.g., `cat /proc/<PID>/maps`). 
 * It is designed to aid in understanding the memory layout of a process. 
 * Useful for educational purposes to see how memory is allocated and modified
 * over time.
 */

void handle_signal(int sig) {
    printf("Signal %d received, exiting now...\n", sig);
    exit(EXIT_SUCCESS);
}

int main(void) {
    struct sigaction sa;
    time_t end = time(NULL) + 240;  /* Set the program to run for about 4 minutes */

    /* Setup the sigaction struct */
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Set up SIGINT / Ctrl-C for graceful termination */
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Process with PID %d is running. Use `cat /proc/%d/maps` to inspect memory layout.\n", getpid(), getpid());

    while (1) {
        if (time(NULL) >= end) {
            break;
        }
        sleep(1); /* Sleep to reduce CPU usage and allow time for manual inspection */
    }

    printf("Completed 4 minutes of runtime. Exiting.\n");

    return EXIT_SUCCESS;
}
