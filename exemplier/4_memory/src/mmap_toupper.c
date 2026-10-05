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
#include <stdio.h>     /* perror(), fprintf() */
#include <stdlib.h>    /* exit() */
#include <ctype.h>     /* toupper() */
#include <fcntl.h>     /* open() */
#include <unistd.h>    /* close() */
#include <sys/mman.h>  /* mmap(), munmap() */
#include <sys/stat.h>  /* fstat() */
#include <sys/types.h> /* off_t */

/**
 * @file mmap_toupper.c
 * @brief Modifies a file in place through a shared projection
 * (MAP_SHARED): converts it to upper case.
 *
 * Lecture: chapter os-09a « Mémoire virtuelle » ("Exemple – modifier un
 * fichier en place").
 *
 * Key points:
 *   - the file is opened in O_RDWR: with MAP_SHARED, PROT_WRITE requires
 *     it (otherwise mmap() fails with EACCES);
 *   - the modifications of the memory area are carried over to the file;
 *   - mmap() returns MAP_FAILED (not NULL) on failure;
 *   - an empty file cannot be projected (length 0 gives EINVAL).
 *
 * \code{.bash}
 *   $ echo bonjour > notes.txt
 *   $ ./mmap_toupper notes.txt
 *   $ cat notes.txt
 *   BONJOUR
 * \endcode
 */

int main(int argc, char *argv[]) {
    const char *nom = (argc > 1) ? argv[1] : "notes.txt";

    int fd = open(nom, O_RDWR);
    if (fd == -1) { perror("open"); exit(EXIT_FAILURE); }

    struct stat st;
    if (fstat(fd, &st) == -1) { perror("fstat"); exit(EXIT_FAILURE); }
    if (st.st_size == 0) {
        fprintf(stderr, "%s : fichier vide\n", nom);
        exit(EXIT_FAILURE);
    }

    char *p = mmap(NULL, st.st_size,
                   PROT_READ | PROT_WRITE,
                   MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); }

    for (off_t i = 0; i < st.st_size; i++) {
        p[i] = toupper((unsigned char) p[i]);
    }

    munmap(p, st.st_size);   /* the file is modified */
    close(fd);

    return EXIT_SUCCESS;
}
