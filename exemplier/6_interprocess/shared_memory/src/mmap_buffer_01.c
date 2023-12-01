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
#include <unistd.h>
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>
#include <sys/mman.h>

/**
 * @file mmap_buffer_01.c
 *
 * Producer-consumer program using a shared memory that stores a single integer.
 * This code is based on the example proposed by:  Janet Davis (2006) and Henry
 * Walker (2004).
 *
 * @author Alain Lebret (2011)
 * @author Janet Davis (2006)
 * @author Henry Walker (2004)
 * @version	1.3
 * @date 2011-12-01
 */

#define INTEGER_SIZE sizeof(int)
#define ITERATIONS 10

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message)
{
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

/**
 * Writes the square of a number onto the shared memory.
 */
void write_memory(void *shared_memory)
{
    int i;

    for (i = 0; i < ITERATIONS; i++) {
        *((int *) shared_memory) = i * i;

        printf("Parent: initial value = %2d\n", i);
        sleep(1);  /* wait 1 sec. to allow his child to read the value */
    }
}

/**
 * Manages the parent process. It writes data to the shared memory and waits
 * for his child to finish.
 */
void manage_parent(void *shared_memory)
{
    pid_t child;
    int status;

    printf("Parent process (PID %d)\n", getpid());
    write_memory(shared_memory);

    child = wait(&status);
    if (WIFEXITED(status)) {
        printf("Parent: child %d has finished (code %d)\n", child,
               WEXITSTATUS(status));
    }
}

/**
 * Reads integers from the shared memory and displays them.
 */
void read_memory(void *shared_memory)
{
    int i;
    int value;

    for (i = 0; i < ITERATIONS; i++) {
        sleep(1);  /* waiting for the memory update (not as good as semaphore) */
        value = *((int *) shared_memory);
        printf("Child: read value = %2d\n", value);
    }
}

/**
 * Manages the child process that reads all data from shared memory.
 */
void manage_child(void *shared_memory)
{
    printf("Child process (PID %d)\n", getpid());
    read_memory(shared_memory);
    printf("Child: memory has been consumed.\n");
}

/**
 * Creates and initializes a new shared memory using the mmap primitive.
 * @return Pointer on the shared memory
 */
void *create_shared_memory()
{
    /* initialize shared memory using mmap */
    void *shared_memory = mmap(0, /* beginning (starting address is ignored) */
                               INTEGER_SIZE, /* size of the shared memory */
                               PROT_READ | PROT_WRITE, /* protection */
                               MAP_SHARED | MAP_ANONYMOUS,
                               -1, /* the shared memory do not use a file */
                               0);  /* ignored: only set when using a file */

    if (shared_memory == (void *) -1) {
        handle_fatal_error("Error [mmap()]: ");
    }
    return shared_memory;
}

int main(void)
{
    pid_t pid;
    void *shared_memory;

    shared_memory = create_shared_memory();

    pid = fork();
    if (pid == -1) {
        handle_fatal_error("Error [fork()]: ");
    }
    if (pid > 0) {
        manage_parent(shared_memory);
    } else {
        manage_child(shared_memory);
    }

    return EXIT_SUCCESS;
}
