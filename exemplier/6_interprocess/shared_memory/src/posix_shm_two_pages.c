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
#include <unistd.h>     /* ftruncate(), close(), fork(), sysconf() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */
#include <sys/wait.h>   /* wait() */

/**
 * @file posix_shm_two_pages.c
 *
 * Example 3 of the lecture: a segment of two pages, projected twice; a
 * structure s1_t in the first page and a structure s2_t in the second one.
 * Lecture: chapter os-09b « Mémoire partagée » ("Exemple 3 – un échange
 * plus complexe").
 *
 * The child writes into both structures, the parent waits for it, then
 * reads. The page size is asked with sysconf(): the offset given to mmap()
 * must be a multiple of it (otherwise EINVAL), and it is 16 KiB on some
 * ARM processors (compare with posix_shm_simple_3.c, which hard-codes 4096).
 * The two projections are not necessarily contiguous.
 * Link with \c -lrt under Linux.
 *
 * \code{.bash}
 *   $ ./posix_shm_two_pages
 *   > 0x7f5c3a2b7000
 *   > 0x7f5c3a2b6000
 *   Monkeypox
 *   112
 * \endcode
 */

typedef struct { float x, y, z; } vector_t;
typedef struct { unsigned char r, g, b; } color_t;

typedef struct {        /* page 0 */
    int id;
    char name[20];
    int age;
} s1_t;

typedef struct {        /* page 1 */
    vector_t vec;
    color_t col;
} s2_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void) {
    long page = sysconf(_SC_PAGESIZE);
    if (page == -1) {
        handle_error("Error [sysconf()]");
    }
    int fd = shm_open("/pipeautique3", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        handle_error("Error [shm_open()]");
    }
    if (ftruncate(fd, 2 * page) == -1) {          /* 2 pages */
        handle_error("Error [ftruncate()]");
    }
    s1_t *p1 = mmap(NULL, sizeof(s1_t),
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);         /* page 0 */
    if (p1 == MAP_FAILED) {
        handle_error("Error [mmap() p1]");
    }
    s2_t *p2 = mmap(NULL, sizeof(s2_t),
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, page);      /* page 1 */
    if (p2 == MAP_FAILED) {
        handle_error("Error [mmap() p2]");
    }
    close(fd);
    printf("> %p\n> %p\n", (void *) p1, (void *) p2);

    fflush(stdout);                             /* before fork() */
    pid_t pid = fork();
    if (pid == -1) {
        handle_error("Error [fork()]");
    }
    if (pid == 0) {                             /* child */
        strcpy(p1->name, "Monkeypox");
        p2->col.r = 112;
        return EXIT_SUCCESS;
    }
    wait(NULL);                                 /* parent */
    printf("%s\n%d\n", p1->name, p2->col.r);

    munmap(p1, sizeof(s1_t));
    munmap(p2, sizeof(s2_t));
    shm_unlink("/pipeautique3");

    return EXIT_SUCCESS;
}
