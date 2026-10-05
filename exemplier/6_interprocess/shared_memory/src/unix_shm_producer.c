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
#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() */
#include <unistd.h> /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>

/**
 * @file unix_shm_producer.c
 *
 * Producer using an IPC/System V shared memory. The program reads a serie of
 * integers and stores their sum in a shared memory.
 *
 * No synchronization: the reader may see data that is only partly updated. See the course, chapter « Synchronisation ».
 */

/**
 * Structure to store the number of \c nb integers and their sum.
 */
struct data {
    int nb;
    int total;
};

typedef struct data data_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void) {
    int id;
    int value;
    key_t key;
    struct data *shared_memory;

    const char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "HOME is not defined: ftok() needs an existing path.\n");
        exit(EXIT_FAILURE);
    }
    key = ftok(home, 'A');
    if (key == -1) {
        handle_fatal_error("Error using ftok()!");
    }

    id = shmget(key, sizeof(data_t), IPC_CREAT | IPC_EXCL | 0600);
    if (id == -1) {
        switch (errno) {
            case EEXIST:
                /* left by a previous run: list it with "ipcs -m", remove it with "ipcrm -m <id>" */
                handle_fatal_error("Segment already exists (ipcs -m, then ipcrm -m <id>)");
                break;
            default:
                handle_fatal_error("Error using shmget()!");
        }
    }

    shared_memory = (data_t *) shmat(id, NULL, 0);  /* 0: read-write attachment */
    if (shared_memory == (void *) -1) {
        handle_fatal_error("Error using shmat()!");
    }

    shared_memory->nb = 0;
    shared_memory->total = 0;

    while (1) {
        printf("+ ");
        if (scanf("%d", &value) != 1 || value == -1) { /* Exit on non-integer or -1 */
            break;
        }
        shared_memory->nb++;
        shared_memory->total += value;
        printf("The sum of %d integers equals %d\n", shared_memory->nb,
               shared_memory->total);
    }
    printf("---\n");

    if (shmdt((char *) shared_memory) == -1) {
        handle_fatal_error("Error using shmdt()!");
    }

    /* remove the memory segment */
    if (shmctl(id, IPC_RMID, NULL) == -1) {
        handle_fatal_error("Error using shmctl()/remove!");
    }

    return EXIT_SUCCESS;
}

