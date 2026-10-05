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
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      /* O_* constants */
#include <unistd.h>     /* ftruncate(), close() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */
#include <sys/stat.h>   /* S_IRUSR, S_IWUSR */

/**
 * @file posix_shm_spacebattle.c
 *
 * Example 1 of the lecture: creates a POSIX shared memory segment, sizes
 * it, projects it, works on it, then frees it.
 * Lecture: chapter os-09b « Mémoire partagée » ("Exemple 1 – créer,
 * projeter, libérer").
 *
 * Life cycle: shm_open() -> ftruncate() -> mmap() -> access -> munmap()
 * -> shm_unlink(). After mmap(), the descriptor can be closed: the
 * projection remains valid.
 * Link with \c -lrt under Linux.
 */

#define SIZE 64

typedef struct {
    int ships;
    int space_grid[SIZE][SIZE][SIZE];
} gameboard_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void) {
    int fd = shm_open("/spacebattle", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        handle_error("Error [shm_open()]");
    }
    if (ftruncate(fd, sizeof(gameboard_t)) == -1) {
        handle_error("Error [ftruncate()]");
    }
    gameboard_t *gb = mmap(NULL, sizeof(gameboard_t),
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED, fd, 0);
    if (gb == MAP_FAILED) {
        handle_error("Error [mmap()]");
    }
    close(fd);

    gb->ships = 3;     /* ... work on the memory */
    gb->space_grid[1][2][3] = 1;
    printf("ships = %d, segment of %zu bytes\n", gb->ships, sizeof(gameboard_t));

    munmap(gb, sizeof(gameboard_t));
    shm_unlink("/spacebattle");

    return EXIT_SUCCESS;
}
