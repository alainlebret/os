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

#define _POSIX_C_SOURCE 200809L /* sigaction() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>

/**
 * @file color_writer_without_synchro.c
 *
 * This program continuously generates random RGB color values and writes
 * them as a formatted string to a shared memory segment. It's designed
 * to run indefinitely, updating the shared memory with new color data 
 * every second.
 *
 * WARNING: synchronization is deliberately missing (no semaphore), so the
 * displayer may read a string that is being written. Compare with
 * color_writer.c.
 */

#define SHM_NAME "/color_memory"
#define SHM_SIZE 1024

/* Set by the SIGINT handler (Ctrl-C): the main loop stops and cleans up */
static volatile sig_atomic_t keep_running = 1;

void handle_sigint(int signum) {
    (void) signum;
    keep_running = 0;
}

/* Function to generate a random integer between min and max */
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main(void) {
    int shm_fd;
    void *shm_ptr;
    char *color_data;

    /* Create or open the shared memory segment */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    /* Set the size of the shared memory segment */
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(1);
    }

    /* Map the shared memory into the address space */
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    /* Pointer to the color data in shared memory */
    color_data = (char *) shm_ptr;


    /* Ctrl-C ends the loop instead of killing the program, so that the
       shared memory is removed below */
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_sigint;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    /* Seed the random number generator with the current time */
    srand(time(NULL));

    while (keep_running) {
        /* Generate random RGB color values */
        int red = random_int(0, 255);
        int green = random_int(0, 255);
        int blue = random_int(0, 255);

        /* Format the color data as a string (e.g., "255,0,0" for red) */
        snprintf(color_data, SHM_SIZE, "%d,%d,%d", red, green, blue);

        /* Print the generated color for debugging */
        printf("Generated Color: %d,%d,%d\n", red, green, blue);

        /* Sleep or do some work */
        sleep(1); /* Sleep for 1 second */
    }

    /* Unmap, close and destroy the shared memory segment when done */
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return EXIT_SUCCESS;
}
