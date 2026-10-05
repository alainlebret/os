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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* random(), exit(), malloc(), free() */
#include <unistd.h>    /* fork(), sleep() */
#include <signal.h>    /* sigaction */
#include <sys/types.h> /* pid_t */

/**
 * @file memory_04.c
 *
 * Demonstrates memory mapping of a process using a heap. Run the program and
 * verify its memory mapping using:
 * \code{bash}
 * cat /proc/<PID>/maps     (Linux; on macOS: vmmap <PID>)
 * \endcode
 * The process is stopped by Ctrl-C before free(): this is not a real leak, the
 * kernel takes back all the memory of a process when it ends. For a leak
 * detected by valgrind, see memory_05a.c.
 */

void handle_signal(int sig) {
    /* Only async-signal-safe functions here: write() and _exit(). The handler
       ends the process at once, so the "flag only" rule of the course is not
       needed: nothing is left half-done in main(). */
    const char msg[] = "\nSignal received, exiting now...\n";
    (void) sig;
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    _exit(EXIT_SUCCESS);
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

    printf("Process with PID %ld is running. Check memory mapping with `cat /proc/%ld/maps`\n", (long) getpid(), (long) getpid());

    /* Continuously write random values to allocated memory */
    while (1) {
        value = random();
        *pointer = value;
        printf("Stored value: %d at address %p\n", *pointer, (void *) pointer);
        sleep(5);
    }

    /* Code to free memory if reached, which it never is in this scenario */
    free(pointer);

    /* Unreachable code due to infinite loop and external termination via signals */
    return EXIT_SUCCESS;
}
