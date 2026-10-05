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
#include <unistd.h>     /* close() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap() */
#include <sys/stat.h>   /* fstat() */

/**
 * @file posix_shm_consumer.c
 *
 * Two unrelated programs sharing memory: the consumer (see
 * posix_shm_creator.c for the other one).
 * Lecture: chapter os-09b « Mémoire partagée » ("Deux programmes – le
 * consommateur").
 *
 * The consumer opens the existing segment "/exemple" read-only (neither
 * O_CREAT nor ftruncate()) and displays its contents.
 * As suggested in the correction of exercise 1, it checks the size of the
 * segment first: reading a segment of 0 byte would raise SIGBUS.
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

int main(void) {
    /* neither O_CREAT, nor ftruncate() */
    int fd = shm_open(NOM, O_RDONLY, 0);
    if (fd == -1) {
        perror("shm_open");
        return EXIT_FAILURE;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return EXIT_FAILURE;
    }
    if (st.st_size < (off_t) sizeof(struct shared_data)) {
        fprintf(stderr, "segment pas encore prêt\n");
        return EXIT_FAILURE;
    }

    struct shared_data *shm = mmap(NULL, sizeof *shm,
                                   PROT_READ,
                                   MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }
    close(fd);

    printf("%d %s\n", shm->valeur, shm->message);

    munmap(shm, sizeof *shm);
    return EXIT_SUCCESS;
}
