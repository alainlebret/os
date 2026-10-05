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
#include <string.h>     /* strcpy() */
#include <fcntl.h>      /* O_* constants */
#include <unistd.h>     /* ftruncate(), close() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */

/**
 * @file posix_shm_creator.c
 *
 * Two unrelated programs sharing memory: the creator (see
 * posix_shm_consumer.c for the other one).
 * Lecture: chapter os-09b « Mémoire partagée » ("Deux programmes – le
 * créateur").
 *
 * The creator creates the segment "/exemple", sizes it, writes a value and
 * a message, then waits for the Enter key before freeing the projection
 * and removing the name.
 * Link with \c -lrt under Linux.
 *
 * \code{.bash}
 *   $ ./posix_shm_creator       # terminal 1: writes, then waits
 *   $ ./posix_shm_consumer      # terminal 2
 *   42 bonjour
 * \endcode
 */

#define NOM "/exemple"

struct shared_data { int valeur; char message[64]; };

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void) {
    int fd = shm_open(NOM, O_CREAT | O_RDWR, 0600);
    if (fd == -1) {
        handle_error("Error [shm_open()]");
    }
    /* The creator sizes the segment BEFORE it is used */
    if (ftruncate(fd, sizeof(struct shared_data)) == -1) {
        handle_error("Error [ftruncate()]");
    }
    struct shared_data *shm = mmap(NULL, sizeof *shm,
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        handle_error("Error [mmap()]");
    }
    close(fd);

    shm->valeur = 42;
    strcpy(shm->message, "bonjour");

    printf("Segment %s ready; press Enter to remove it.\n", NOM);
    getchar();              /* waits for the Enter key */

    munmap(shm, sizeof *shm);
    shm_unlink(NOM);

    return EXIT_SUCCESS;
}
