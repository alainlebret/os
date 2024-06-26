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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>

/**
 * @file posix_shm_client.c
 *
 * Example using a server and a client sharing memory.
 * Link with \c -lrt.
 */

#define MAC_OSX 0
#define MEMORY_PATH "/shm_name"

/**
 * Structure to store a value and its square root.
 */
struct memory_t {
    int value;
    double square_root;
};

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Unlinks the shared memory when receiving the SIGINT signal.
 */
void handle_sigint(int signum) {
    if (shm_unlink(MEMORY_PATH) < 0) {
        handle_error("Error [shm_unlink()]: ");
    }
    exit(EXIT_SUCCESS);
}

/**
 * Checks if new data is available in the shared memory.
 *
 * @param memory The shared memory segment.
 * @param last_read The last read data from the shared memory.
 * @return 1 if new data is available, 0 otherwise.
 */
int check_for_new_data(struct memory_t *memory, struct memory_t *last_read) {
    /* Check if the current data is different from the last read data */
    if (memory->value != last_read->value ||
        memory->square_root != last_read->square_root) {
        /* Update last_read to the current data */
        last_read->value = memory->value;
        last_read->square_root = memory->square_root;
        return 1; /* New data is available */
    }
    return 0; /* No new data */
}

int main(int argc, char *argv[]) {
    int memory_descriptor;
    size_t memory_size;
    struct memory_t *memory;
    struct memory_t last_read = {0, 0};
    struct sigaction action;

    memory_size = (1 * sizeof(struct memory_t));
    action.sa_handler = &handle_sigint;

    sigaction(SIGINT, &action, NULL);

    memory_descriptor = shm_open(
            MEMORY_PATH,
            O_RDWR,
            S_IRWXU | S_IRWXG);
    if (memory_descriptor < 0) {
        handle_error("Error [shm_open()]: ");
    }

    fprintf(stderr, "Shared memory object %s has been opened", MEMORY_PATH);

    if (MAC_OSX) { /* MAC OS X abnormality about ftruncate */
        struct stat mapstat;
        if (fstat(memory_descriptor, &mapstat) == -1) {
            handle_error("Error [fstat()]: ");
        }

        if (mapstat.st_size == 0) {
            if (ftruncate(memory_descriptor, memory_size) == -1) {
                handle_error("Error [ftruncate()]: ");
            }
        }
    } else {
        if (ftruncate(memory_descriptor, memory_size) == -1) {
            handle_error("Error [ftruncate()]: ");
        }
    }

    memory = (struct memory_t *) mmap(
            NULL,
            memory_size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            memory_descriptor,
            0);
    if (memory == MAP_FAILED) {
        handle_error("Error [mmap()]: ");
    }
    fprintf(stderr, "Shared memory of %zu bytes has been allocated\n",
            memory_size);

    while (1) {
        printf("Value is %d ... ", memory->value);
        printf("and its square root is %f \n", memory->square_root);
        if (check_for_new_data(memory, &last_read)) {
            printf("New data read from shared memory.\n");
        }
        sleep(3);
    }

    if (munmap(memory, memory_size) == -1) {
        perror("munmap");
    }
}
