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
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>

/**
 * @file color_modifier.c
 * @brief A program that modifies color data in shared memory.
 *
 * This program fills a matrix in shared memory with random color values.
 * The matrix size is passed as a command-line argument. The program
 * uses POSIX shared memory and updates the colors at regular intervals.
 */

#define MAX_MATRIX_SIZE 800

volatile sig_atomic_t keep_running = 1;

/**
 * Signal handler for SIGINT (Ctrl-C).
 *
 * @param sig Signal number.
 */
void handle_sigint(int sig) {
    keep_running = 0;
}

int main(int argc, char *argv[]) {
    int shm_fd;
    int *ptr;
    int i, j;
    int MATRIX_SIZE;
    struct sigaction action;

    /* Check if matrix size is passed as an argument */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <size_of_the_shared_memory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Convert the argument to an integer */
    MATRIX_SIZE = atoi(argv[1]);
    if (MATRIX_SIZE > MAX_MATRIX_SIZE) {
        fprintf(stderr, "Matrix size should not exceed %d\n", MAX_MATRIX_SIZE);
        exit(EXIT_FAILURE);
    }

    shm_unlink("/matrix");

    /* Set up the structure for signal handling */
    memset(&action, '\0', sizeof(action));
    action.sa_handler = &handle_sigint;
    sigaction(SIGINT, &action, NULL);

    /* Open a shared memory segment */
    shm_fd = shm_open("/matrix", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /* Set the size of the shared memory segment */
    if (ftruncate(shm_fd, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    /* Map the shared memory segment into the address space */
    ptr = mmap(0, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* Initialize the random number generator */
    srand(time(NULL));

    /* Main loop to fill the matrix with random color values */
    while (keep_running) {
        for (i = 0; i < MATRIX_SIZE; i++) {
            for (j = 0; j < MATRIX_SIZE; j++) {
                ptr[(i * MATRIX_SIZE + j) * 3] = rand() % 256; /* Red */
                ptr[(i * MATRIX_SIZE + j) * 3 + 1] = rand() % 256; /* Green */
                ptr[(i * MATRIX_SIZE + j) * 3 + 2] = rand() % 256; /* Blue */
            }
        }
        usleep(500000); /* Wait for 500 ms */
    }

    /* Detach the shared memory segment */
    if (munmap(ptr, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    /* Close the shared memory file descriptor */
    close(shm_fd);
    shm_unlink("/matrix");

    return EXIT_SUCCESS;
}
