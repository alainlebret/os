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
#include <stdio.h>      /* printf(), perror() */
#include <stdlib.h>     /* exit() */
#include <fcntl.h>      /* O_* constants */
#include <unistd.h>     /* ftruncate(), fork(), close() */
#include <semaphore.h>  /* sem_open(), sem_wait(), sem_post(), sem_close() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */
#include <sys/wait.h>   /* wait() */

/**
 * @file cnt_fork_named_semaphore.c
 *
 * Running example, correction 1, fork version: two child processes
 * increment a counter in a POSIX shared memory segment, protected by a
 * named POSIX semaphore.
 * Lecture: chapter os-10 « Synchronisation », appendices ("Fil rouge –
 * correction 1, version fork" and "Sémaphore nommé – créer et observer").
 *
 * The named semaphore is opened before fork(): the children inherit it.
 * Under Linux, it is visible as /dev/shm/sem.cnt-sem until sem_unlink().
 * Failed version: ../../various/src/cnt_fork.c.
 * Works under Linux and macOS. Beware: under macOS, named semaphores are
 * very slow under contention (about 9 minutes measured for NB_ITERS =
 * 100000000): compile with e.g. -DNB_ITERS=1000000 for a quick test.
 * Link with \c -lrt -pthread under Linux.
 *
 * \code{.bash}
 *   $ ./cnt_fork_named_semaphore
 *   cnt=200000000
 * \endcode
 */

#ifndef NB_ITERS
#define NB_ITERS 100000000
#endif

/**
 * Handles a fatal error by displaying a message, then exits.
 */
void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void) {
    sem_unlink("/cnt-sem");        /* removes a semaphore left by a crash */
    sem_t *sem = sem_open("/cnt-sem", O_CREAT, 0600, 1);
    if (sem == SEM_FAILED) {
        handle_fatal_error("sem_open");
    }

    shm_unlink("/cnt");            /* removes a segment left by a crash */
    int fd = shm_open("/cnt", O_CREAT | O_RDWR, 0600);
    if (fd == -1) {
        handle_fatal_error("shm_open");
    }
    if (ftruncate(fd, sizeof(unsigned int)) == -1) {  /* set to zero */
        handle_fatal_error("ftruncate");
    }
    unsigned int *cnt = mmap(NULL, sizeof *cnt,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED, fd, 0);
    if (cnt == MAP_FAILED) {
        handle_fatal_error("mmap");
    }
    close(fd);

    for (int k = 0; k < 2; k++) {
        pid_t pid = fork();
        if (pid == -1) {
            handle_fatal_error("fork");
        }
        if (pid == 0) {            /* the child inherits sem */
            for (int i = 0; i < NB_ITERS; i++) {
                sem_wait(sem);
                (*cnt)++;
                sem_post(sem);
            }
            exit(EXIT_SUCCESS);
        }
    }
    wait(NULL);
    wait(NULL);
    printf("cnt=%u\n", *cnt);      /* 200000000 */

    sem_close(sem);
    sem_unlink("/cnt-sem");
    munmap(cnt, sizeof *cnt);
    shm_unlink("/cnt");

    return EXIT_SUCCESS;
}
