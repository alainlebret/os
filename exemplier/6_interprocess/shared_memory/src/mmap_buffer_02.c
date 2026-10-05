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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> /* wait() */
#include <sys/mman.h>

/**
 * @file mmap_buffer_02.c
 *
 * Producer-consumer program using a shared memory that stores a buffer of
 * integers. The busy waiting on shared indices only illustrates the problem
 * (volatile prevents the compiler from caching them, but it is neither
 * atomic nor efficient): see mmap_buffer_03.c for semaphores.
 * This code is based on the example presented by:  Janet Davis (2006)
 * and Henry Walker (2004).
 *
 * @author Alain Lebret (2011)
 * @author Janet Davis (2006)
 * @author Henry Walker (2004)
 * @version	1.3
 * @date 2011-12-01
 */

#define INTEGER_SIZE sizeof(int)
#define BUFFER_SIZE 5
#define MEMORY_SIZE (BUFFER_SIZE+2)*INTEGER_SIZE
#define ITERATIONS 10

/**
 * Handles a fatal error. It displays the message followed by the reason
 * given by errno (perror()), then exits.
 */
void handle_fatal_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Writes the square of a series of integers onto the shared memory.
 * This function implements a simple busy-waiting mechanism to ensure
 * synchronization between producer and consumer.
 */
void write_memory(volatile int *buffer, volatile int *begin, volatile int *end) {
    int i;

    for (i = 0; i < ITERATIONS; i++) {
        while ((*begin + 1) % BUFFER_SIZE == *end) {}

        buffer[*begin] = i * i;
        *begin = (*begin + 1) % BUFFER_SIZE;

        printf("Parent: initial value %2d before writing in the buffer\n", i);
    }
}

/**
 * Manages the parent process. It writes data to the shared memory and waits
 * for his child to finish.
 */
void manage_parent(volatile int *buffer, volatile int *begin, volatile int *end) {
    pid_t child;
    int status;

    printf("Parent process (PID %ld)\n", (long) getpid());
    write_memory(buffer, begin, end);
    printf("Parent: end of production.\n");

    child = wait(&status);
    if (WIFEXITED(status)) {
        printf("Parent: child %ld has finished (code %d)\n", (long) child,
               WEXITSTATUS(status));
    }
}

/**
 * Reads a series of integers from the shared memory and displays them.
 * This function implements a simple busy-waiting mechanism for synchronization.
 */
void read_memory(volatile int *buffer, volatile int *in, volatile int *out) {
    int i;
    int value;

    for (i = 0; i < ITERATIONS; i++) {
        sleep(1);  /* slows the reader down, so that the buffer fills up */

        while (*in == *out) {}  /* active wait: buffer empty (a semaphore avoids this) */

        value = buffer[*out];
        *out = (*out + 1) % BUFFER_SIZE;
        printf("Child: element %2d == %2d read from the buffer.\n", i, value);
    }
}

/**
 * Manages the child process that reads all data from shared memory.
 */
void manage_child(volatile int *buffer, volatile int *in, volatile int *out) {
    printf("Child process (PID %ld)\n", (long) getpid());
    read_memory(buffer, in, out);
    printf("Child: memory has been consumed.\n");
}

/**
 * Creates and initializes a new shared memory using mmap.
 * @return Pointer on the shared memory
 */
void *create_shared_memory(void) {
    /* initialize shared memory using mmap */
    void *shared_memory = mmap(0, /* beginning (starting address is ignored) */
                               MEMORY_SIZE, /* size of the shared memory */
                               PROT_READ | PROT_WRITE, /* protection */
                               MAP_SHARED | MAP_ANONYMOUS,
                               -1, /* the shared memory do not use a file */
                               0);  /* ignored: set when using a file */

    if (shared_memory == MAP_FAILED) {
        handle_fatal_error("Error allocating shared memory using mmap");
    }
    return shared_memory;
}

int main(void) {
    pid_t pid;
    volatile int *buffer; /* logical base address for buffer */
    volatile int *in;     /* pointer to logical 'in' address for producer */
    volatile int *out;    /* pointer to logical 'out' address for consumer */
    void *shared_memory; /* shared memory base address */

    shared_memory = create_shared_memory();

    /* The segment of shared memory is organised as:
     *  0                                                n-1   n   n+1
     * ----------------------------------------------------------------
     * |                                                    |    |    |
     * ----------------------------------------------------------------
     *  ^                                                    ^    ^
     *  buffer                                               in   out
     */

    buffer = (volatile int *) shared_memory;
    in = buffer + BUFFER_SIZE;       /* pointer arithmetic counts in ints, */
    out = buffer + BUFFER_SIZE + 1;  /* not in bytes */

    *in = *out = 0;          /* starting index */

    pid = fork();
    if (pid == -1) {
        handle_fatal_error("Error [fork()]");
    }
    if (pid == 0) {
        manage_child(buffer, in, out);
        /* Child process cleanup */
        if (munmap(shared_memory, MEMORY_SIZE) == -1) {
            perror("munmap");
        }
    } else {
        manage_parent(buffer, in, out);
        /* Parent process cleanup */
        if (munmap(shared_memory, MEMORY_SIZE) == -1) {
            perror("munmap");
        }
    }

    return EXIT_SUCCESS;
}
