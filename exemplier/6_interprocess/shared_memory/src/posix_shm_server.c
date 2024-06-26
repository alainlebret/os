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
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * @file posix_shm_server.c
 *
 * Example using a server and a client sharing memory.
 * Link with \c -lrt under Linux.
 */

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
    fprintf(stderr, "Shared memory %s unlinked successfully.\n", MEMORY_PATH);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int memory_descriptor;
    int value;
    size_t memory_size;
    struct memory_t *memory;
    struct sigaction action;

    memory_size = (1 * sizeof(struct memory_t));

    action.sa_handler = &handle_sigint;

    sigaction(SIGINT, &action, NULL);

    memory_descriptor = shm_open(
            MEMORY_PATH,
            O_CREAT | O_RDWR,
            S_IRWXU | S_IRWXG);
    if (memory_descriptor < 0) {
        handle_error("Error [shm_open()]: ");
    }

    fprintf(stderr, "Shared memory object %s has been created\n", MEMORY_PATH);

    if (ftruncate(memory_descriptor, memory_size) == -1) {
        handle_error("Error [ftruncate()]: ");
    }

    memory = (struct memory_t *) mmap(
            NULL,
            memory_size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            memory_descriptor,
            0);
    if (memory == MAP_FAILED) {
        handle_error("Error [mmap()]: ");;
    }
    fprintf(stderr, "Memory of %zu bytes allocated.\n", memory_size);

    value = 1;

    while (1) {
        memory->value = value;
        memory->square_root = sqrt(value);
        fprintf(stderr, "Updated shared memory: value = %d, square root = %f\n",
                memory->value, memory->square_root);
        sleep(5);
        value++;
    }

    munmap(memory, memory_size);
    close(memory_descriptor);
}
