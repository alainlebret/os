/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * Chapter "Interprocess communication" / Chapitre "Communication interprocessus"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2016-11-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <signal.h>

#define MAC_OSX 0
#define FOREVER for (;;)
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
void handle_error(char *message)
{
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

/**
 * Unlinks the shared memory when receiving the SIGINT signal.
 */
void handle_sigint(int signum)
{
    if (shm_unlink(MEMORY_PATH) < 0) {
        handle_error("Error calling shm_unlink\n");
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int memory_descriptor;
    size_t memory_size = (1 * sizeof(struct memory_t));
    struct memory_t *memory;
    struct sigaction action;
    action.sa_handler = &handle_sigint;

    sigaction(SIGINT, &action, NULL);

    memory_descriptor = shm_open(
            MEMORY_PATH,
            O_RDWR,
            S_IRWXU | S_IRWXG);
    if (memory_descriptor < 0) {
        handle_error("Error calling shm_open\n");
    }

    fprintf(stderr, "Shared memory object %s has been opened", MEMORY_PATH);

    if (MAC_OSX) { /* MAC OS X abnormality about ftruncate */
        struct stat mapstat;
        if (-1 != fstat(memory_descriptor, &mapstat)
            && mapstat.st_size == 0) {
            ftruncate(memory_descriptor, memory_size);
        }
    } else {
        ftruncate(memory_descriptor, memory_size);
    }

    memory = (struct memory_t *) mmap(
            NULL,
            memory_size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            memory_descriptor,
            0);
    if (memory == MAP_FAILED) {
        handle_error("Error calling mmap\n");
    }
    fprintf(stderr, "Shared memory of %zu bytes has been allocated\n",
            memory_size);

    FOREVER {
        printf("Value is %d ... ", memory->value);
        printf("and its square root is %f \n", memory->square_root);
        sleep(3);
    }

    /*
     * Unreachable: use a signal handler with the following code
     * if (shm_unlink(MEMORY_PATH) != 0) {
     *    handle_error("Error calling shm_unlink\n");
     * }
     * exit(EXIT_SUCCESS);
     */
}
