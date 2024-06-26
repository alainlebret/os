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
#include <stdlib.h>    /* random(), exit(), malloc(), free() */
#include <unistd.h>    /* fork(), sleep() */
#include <signal.h>    /* sigaction */
#include <sys/types.h> /* pid_t, mkfifo() */

/**
 * @file memory_04.c
 *
 * Demonstrates memory mapping of a process using a heap and highlights potential
 * memory leak problems. Run the program and verify its memory mapping using:
 * \code{bash}
 * cat /proc/<PID>/maps
 * \endcode
 * Also, compile with -g and check for memory leaks using valgrind.
 */

void handle_signal(int sig) {
    printf("\nSignal %d received, cleaning up and exiting now...\n", sig);
    exit(EXIT_SUCCESS);
}

int main(void) {
    struct sigaction sa;
    int *pointer;
    int value;

    /* Setup the sigaction struct to handle SIGINT (Ctrl-C) */
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Apply the sigaction configuration */
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up sigaction");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory on the heap */
    pointer = (int *) malloc(sizeof(int));
    if (pointer == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    printf("Process with PID %d is running. Check memory mapping with `cat /proc/%d/maps`\n", getpid(), getpid());

    /* Continuously write random values to allocated memory */
    while (1) {
        value = random();
        *pointer = value;
        printf("Stored value: %d at address %p\n", *pointer, pointer);
        sleep(5);
    }

    /* Code to free memory if reached, which it never is in this scenario */
    free(pointer);

    /* Unreachable code due to infinite loop and external termination via signals */
    return EXIT_SUCCESS;
}
