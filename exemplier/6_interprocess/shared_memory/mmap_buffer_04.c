/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h> /* wait() */
#include <sys/wait.h> /* wait() */
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NBR_CONSUMERS 5  /* number of consumer processes to be spawned */
#define NBR_PRODUCTERS 6  /* number of producer processes to be spawned */

#define INTEGER_SIZE sizeof(int)
#define BUFFER_SIZE 5
#define MEMORY_SIZE (BUFFER_SIZE+2)*INTEGER_SIZE

#define ITERATIONS_CONSUMER 12  /* number of iterations for consumer in test run */
#define ITERATIONS_PRODUCER 10  /* number of iterations for producer in test run */

#define BUFFER_USED 0     /* semaphore array index to check buffer elts used */
#define BUFFER_SPACE 1    /* semaphore array index to check buffer elts empty */
#define MUTEX 2        /* semaphore index for mutual exclusion to buffer*/
#define NBR_SEMAPHORES 3     /* number of semaphores */

/**
 * @author Alain Lebret (2011)
 * @author Janet Davis (2006)
 * @author Henry Walker (2004)
 * @version	1.3
 * @date 2011-12-01
 */

/**
 * @file mmap_buffer_04.c
 *
 * Multi-producer-multi-consumer program using a shared memory and synchronized
 * with semaphores. This code is based on the example presented by:  Janet Davis
 * (2006) and Henry Walker (2004).
 */

 /**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message)
{
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

/**
 * Creates a set of semaphores.
 * @param nbr_semaphores The number of semaphores to create.
 * @return Identifier of the set of semaphores
 */
int create_semaphores(int nbr_semaphores)
{
    int semid;

    /* permission 0600 = lecture/modification by user */
    if ((semid = semget(IPC_PRIVATE, nbr_semaphores, IPC_CREAT | 0600))
        < 0) {
        handle_fatal_error("Error when creating semaphores! ");
    }

    return semid;
}

/**
 * Initializes a specific semaphore.
 */
void initialize_semaphore(int semid, int index, int valeur)
{
    if (semctl(semid, index, SETVAL, valeur) < 0) {
        handle_fatal_error("Error when initializing semaphore. ");
    }
}

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param semid Identifier of the group of semaphores.
 * @param index Index of the semaphore in the group.
 */
void sem_wait(int semid, int index)
{
    struct sembuf sops[1];

    sops[0].sem_num = index;
    sops[0].sem_op = -1;
    sops[0].sem_flg = 0;

    if (semop(semid, sops, 1) < 0) {
        handle_fatal_error("Error using P(). ");
    }
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param semid Identifier of the group of semaphores.
 * @param index Index of the semaphore in the group.
 */
void sem_signal(int semid, int index)
{
    struct sembuf sops[1];

    sops[0].sem_num = index;
    sops[0].sem_op = 1;
    sops[0].sem_flg = 0;

    if (semop(semid, sops, 1) < 0) {
        handle_fatal_error("Error using V(). ");
    }
}

/**
 * Writes the square of a serie of integers onto the shared memory.
 */
void write_memory(int id, int *buffer, int *in, int *out, int semid)
{
    int value;
    int i;

    for (i = 0; i < ITERATIONS_PRODUCER; i++) {
        value = 100 * id + i;
        sem_wait(semid, BUFFER_SPACE);  /* P() -- wait */
        sem_wait(semid, MUTEX);    /* Waiting to access buffer */
        buffer[*in] = value;
        *in = (*in + 1) % BUFFER_SIZE;
        sem_signal(semid, MUTEX);  /* End of access */
        sem_signal(semid, BUFFER_USED);  /* V() -- signal */
    }
}

/**
 * Writes data to the shared memory.
 */
void produce(int id, int *buffer, int *in, int *out, int semid)
{
    printf("Producer of ID %d begins.\n", id);
    write_memory(id, buffer, in, out, semid);
    printf("Producer of ID %d has finished.\n", id);
}

/**
 * Reads and reports values from shared memory buffer.
 */
void consume(int id, int *buffer, int *in, int *out, int semid)
{
    int i;
    int value;

    printf("Consumer of ID %d begins.\n", id);

    for (i = 0; i < ITERATIONS_CONSUMER; i++) {
        sem_wait(semid, BUFFER_USED);  /* wait semaphore for something used */
        sem_wait(semid, MUTEX);     /* wait semaphore for buffer access */
        value = buffer[*in];  /* take data from buffer */
        *in = (*in + 1) % BUFFER_SIZE;
        sem_signal(semid, MUTEX);     /* signal semaphore for buffer access */
        sem_signal(semid,
                   BUFFER_SPACE); /* signal semaphore for space available */

        printf("Consumer of ID %d : element %2d == %2d\n", id, i, value);

        if ((i + id) % 5 == 0) /* pause somewhere in processing */
            sleep(1);               /* to make output more interesting */
    }
    printf("Consumer of ID %d has finished.\n", id);
}

/**
 * Creates and initializes a new shared memory using mmap.
 * @return Pointer on the shared memory
 */
void *create_shared_memory()
{
    /* initialize shared memory using mmap */
    void *shared_memory = mmap(0, /* beginning (starting address is ignored) */
                               MEMORY_SIZE, /* size of the shared memory */
                               PROT_READ | PROT_WRITE, /* protection */
                               MAP_SHARED | MAP_ANONYMOUS,
                               -1, /* the shared memory do not use a file */
                               0);  /* ignored: set when using a file */

    if (shared_memory == (void *) -1) {
        handle_fatal_error("Error allocating shared memory using mmap!\n");
    }
    return shared_memory;
}

int main(void)
{
    pid_t pid;
    int i;
    int semid; /* identifier for a semaphore set */
    void *shared_memory; /* shared memory base address */
    int *buffer; /* logical base address for buffer */
    int *in; /* pointer to logical 'in' address for producer */
    int *out; /* pointer to logical 'out' address for consumer */


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
    in = (int *) shared_memory + BUFFER_SIZE * INTEGER_SIZE;
    out = (int *) shared_memory + (BUFFER_SIZE + 1) * INTEGER_SIZE;

    *in = *out = 0;          /* starting index */

    /* Creation and initialization of semaphores */
    semid = create_semaphores(NBR_SEMAPHORES);
    initialize_semaphore(semid, BUFFER_USED, 0);
    initialize_semaphore(semid, BUFFER_SPACE, BUFFER_SIZE);
    initialize_semaphore(semid, MUTEX, 1);

    /* Creation of producers */
    for (i = 1; i <= NBR_PRODUCTERS; i++) {
        pid = fork();
        if (pid == -1) {
            handle_fatal_error("Error when trying to fork producers. ");
        }
        if (pid == 0) {
            produce(i, buffer, in, out, semid);
            exit(EXIT_SUCCESS);
        }
    }

    /* Creation of consumers */
    for (i = 1; i <= NBR_CONSUMERS; i++) {
        pid = fork();
        if (pid == -1) {
            handle_fatal_error("Error when trying to fork consumers. ");
        }

        if (pid == 0) {
            consume(i, buffer, in, out, semid);
            exit(EXIT_SUCCESS);
        }
    }

    /* Waiting for the end of all consumers */
    printf("All producers and consumers are working...\n");
    printf("Parent process is waiting for them to end...\n");

    for (i = 0; i < NBR_PRODUCTERS + NBR_CONSUMERS; i++) {
        wait(NULL);
    }

    /* Remove the semaphore from the system and destroy the set of
     *  semaphores and data structure associated with it. */
    if (semctl(semid, 0, IPC_RMID) < 0) {
        handle_fatal_error("Error removing semaphores. ");
        exit(EXIT_FAILURE);
    }
    printf("Semaphores removed.\n");

    exit(EXIT_SUCCESS);
}

