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
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>

/**
 * @file mmap_buffer_03.c
 *
 * Producer-consumer program using a shared memory that stores a buffer of
 * integers and synchronized using semaphores. This code is based on the example
 * presented by:  Janet Davis (2006) and Henry Walker (2004).
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

#define BUFFER_USED  0
#define BUFFER_SPACE 1
#define NBR_SEMAPHORES  2

/**
 * Handles a fatal error. It displays the message followed by the reason
 * given by errno (perror()), then exits.
 */
void handle_fatal_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Creates and initializes a group of two semaphores.
 * @return Identifier of the group of semaphores
 */
int initialize_semaphores(void) {
    int semid;

    /* permission 0600 = lecture/modification by user */
    if ((semid = semget(IPC_PRIVATE, NBR_SEMAPHORES, IPC_CREAT | 0600))
        < 0) {
        handle_fatal_error("Error [semget()]");
    }

    /* POSIX passes the 4th argument of semctl(SETVAL) as a union semun
       (field val), which the program must define itself on Linux; passing
       an int works on the usual ABIs (Linux, macOS), as here. */
    if (semctl(semid, BUFFER_SPACE, SETVAL, BUFFER_SIZE) < 0) {
        handle_fatal_error("Error first [semctrl()]");
    }

    if (semctl(semid, BUFFER_USED, SETVAL, 0) < 0) {
        handle_fatal_error("Error second [semctrl()]");
    }

    return semid;
}

/**
 * Performs a P() operation ("wait") on a System V semaphore.
 * Not named sem_wait(): that name belongs to POSIX semaphores (<semaphore.h>).
 * @param semid Identifier of the group of two semaphores.
 * @param index Index of the semaphore in the group.
 */
void P(int semid, int index) {
    struct sembuf sops[1];

    sops[0].sem_num = index;
    sops[0].sem_op = -1;
    sops[0].sem_flg = 0;

    if (semop(semid, sops, 1) < 0) {
        handle_fatal_error("Error using P()");
    }
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param semid Identifier of the group of two semaphores.
 * @param index Index of the semaphore in the group.
 */
void V(int semid, int index) {
    struct sembuf sops[1];

    sops[0].sem_num = index;
    sops[0].sem_op = 1;
    sops[0].sem_flg = 0;

    if (semop(semid, sops, 1) < 0) {
        handle_fatal_error("Error using V()");
    }
}

/**
 * Writes the square of a serie of integers onto the shared memory.
 */
void write_memory(int *buffer, int *in, int *out, int semid) {
    int i;

    (void) out;  /* only used by the consumer */
    for (i = 0; i < ITERATIONS; i++) {
        P(semid, BUFFER_SPACE);  /* P() -- wait */

        buffer[*in] = i * i;
        *in = (*in + 1) % BUFFER_SIZE;
        printf("Parent: initial value %2d before writing in the buffer\n", i);

        V(semid, BUFFER_USED); /* V() -- signal */

        if ((i % 4) == 0) {
            sleep(1); /* slow production */
        }
    }
}

/**
 * Manages the parent process. It writes data to the shared memory and waits
 * for his child to finish.
 */
void manage_parent(int *buffer, int *in, int *out, int semid) {
    pid_t child;
    int status;

    printf("Parent process (PID %ld)\n", (long) getpid());
    write_memory(buffer, in, out, semid);
    printf("Parent: end of production.\n");

    child = wait(&status);
    if (WIFEXITED(status)) {
        printf("Parent: child %ld has finished (code %d)\n", (long) child,
               WEXITSTATUS(status));
    }
}

/**
 * Reads a serie of integers from the shared memory and displays them.
 */
void read_memory(int *buffer, int *in, int *out, int semid) {
    int i;
    int value;

    (void) in;  /* only used by the producer */
    for (i = 0; i < ITERATIONS; i++) {
        P(semid, BUFFER_USED);  /* P() -- wait */

        value = buffer[*out];
        *out = (*out + 1) % BUFFER_SIZE;
        printf("Child: element %2d == %2d read from the buffer.\n", i, value);

        V(semid, BUFFER_SPACE); /* V() -- signal */

        if ((i % 3) == 1) {
            sleep(1); /* slow consuming */
        }
    }
}

/**
 * Manages the child process that reads all data from shared memory.
 */
void manage_child(int *buffer, int *in, int *out, int semid) {
    printf("Child process (PID %ld)\n", (long) getpid());
    read_memory(buffer, in, out, semid);
    printf("Child: memory has been totally consumed.\n");
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
        handle_fatal_error("Error [mmap()]");
    }
    return shared_memory;
}

int main(void) {
    pid_t pid;

    void *shared_memory; /* shared memory base address */
    int *buffer; /* logical base address for buffer */
    int *in; /* pointer to logical 'in' address for producer */
    int *out; /* pointer to logical 'out' address for consumer */
    int semaphores; /* identifier for a semaphore set */

    shared_memory = create_shared_memory();

    /* The segment of shared memory is organised as:
     *  0                                                n-1   n   n+1
     * ----------------------------------------------------------------
     * |                                                    |    |    |
     * ----------------------------------------------------------------
     *  ^                                                    ^    ^
     *  buffer                                               in   out
     */

    buffer = (int *) shared_memory;
    in = buffer + BUFFER_SIZE;       /* pointer arithmetic counts in ints, */
    out = buffer + BUFFER_SIZE + 1;  /* not in bytes */

    *in = *out = 0;          /* starting index */

    semaphores = initialize_semaphores();

    pid = fork();
    if (pid == -1) {
        handle_fatal_error("Error [fork()]");
    }
    if (pid == 0) {
        manage_child(buffer, in, out, semaphores);
    } else {
        manage_parent(buffer, in, out, semaphores); /* waits for the child */

        if (semctl(semaphores, 0, IPC_RMID) < 0) {
            handle_fatal_error("Error when trying to remove semaphores");
        }
        printf("Semaphores have been removed.\n");
    }

    return EXIT_SUCCESS;
}
