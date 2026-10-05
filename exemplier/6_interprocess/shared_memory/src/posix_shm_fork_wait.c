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
#include <stdlib.h>     /* srand(), rand() */
#include <time.h>       /* time() */
#include <fcntl.h>      /* O_* constants */
#include <unistd.h>     /* ftruncate(), close(), fork() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */
#include <sys/wait.h>   /* wait() */

/**
 * @file posix_shm_fork_wait.c
 *
 * Example 2 of the lecture: a parent and a child sharing an array of 25
 * integers; the child writes, the parent waits for it, then reads.
 * Lecture: chapter os-09b « Mémoire partagée » ("Exemple 2 – un parent et
 * un enfant").
 *
 * The projection is made before fork(): the child inherits it and uses the
 * same physical frames. wait() guarantees that the parent reads after the
 * writing of the child (compare with posix_shm_simple_1.c, which has no
 * synchronization).
 * Link with \c -lrt under Linux.
 *
 * \code{.bash}
 *   $ ./posix_shm_fork_wait
 *   6 3 4 3 0 8 3 6 4 5 1 9 2 7 7 0 5 2 8 1 4 6 9 3 2
 * \endcode
 */

#define N      25           /* 25 integers: 100 bytes */
#define TAILLE (N * sizeof(int))

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main(void) {
    int fd = shm_open("/exemple", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        handle_error("Error [shm_open()]");
    }
    if (ftruncate(fd, TAILLE) == -1) {
        handle_error("Error [ftruncate()]");
    }
    int *tab = mmap(NULL, TAILLE, PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);
    if (tab == MAP_FAILED) {
        handle_error("Error [mmap()]");
    }
    close(fd);

    srand(time(NULL));

    pid_t pid = fork();
    if (pid == -1) {
        handle_error("Error [fork()]");
    }
    if (pid == 0) {                   /* child: writes */
        for (int i = 0; i < N; i++) {
            tab[i] = rand() % 10;
        }
        munmap(tab, TAILLE);
        return EXIT_SUCCESS;
    }

    wait(NULL);                       /* parent: waits */
    for (int i = 0; i < N; i++) {
        printf("%d ", tab[i]);
    }
    printf("\n");

    munmap(tab, TAILLE);
    shm_unlink("/exemple");

    return EXIT_SUCCESS;
}
