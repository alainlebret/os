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
#include <stdlib.h>    /* exit(), malloc(), free() */
#include <unistd.h>    /* pause() */
#include <signal.h>    /* for sigaction */
#include <sys/types.h> /* pid_t */

/**
 * @file memory_05b.c
 *
 * This program allocates memory for an array of 10 integers and then pauses, 
 * creating an intentional memory leak for analysis with memory profiling tools
 * like Valgrind. Compile it with -g option and run the Valgrind program.
 */

void handle_signal(int sig) {
    printf("Signal %d received. Exiting and cleaning up...\n", sig);
    exit(EXIT_SUCCESS);
}

int main(void) {
    int *p;
    struct sigaction sa;

    /* Clear the sigaction structure */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_signal;

    /* Set up sigaction for SIGINT */
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up sigaction for SIGINT");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for 10 integers */
    p = (int *) malloc(10 * sizeof(int));
    if (!p) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    /* Inform user about how to proceed */
    printf("Program paused. PID: %d. Use Ctrl-C to exit.\n", getpid());

    /* Pause the program, waiting for signal */
    pause();

    /* Free allocated memory if reached (not in this code as pause() is indefinite) */
    free(p);

    return EXIT_SUCCESS;
}
