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
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */
#include <sys/wait.h>   /* wait() */

/**
 * @file cnt_fork.c
 *
 * Running example, failed synchronization (fork version): two child
 * processes increment a counter placed in a POSIX shared memory segment,
 * NB_ITERS times each, without protection.
 * Lecture: chapter os-10 « Synchronisation » ("Synchronisation ratée –
 * version fork").
 *
 * Same defect as with threads (cnt_thread.c): cnt should be 200 000 000,
 * but the result is wrong and changes at each execution.
 * The counter is accessed through a volatile pointer so that the optimizer
 * does not merge the loop. Correction with a named semaphore:
 * ../semaphores/src/cnt_fork_named_semaphore.c.
 * Link with \c -lrt under Linux.
 *
 * \code{.bash}
 *   $ ./cnt_fork
 *   cnt=65486848
 *   $ ./cnt_fork
 *   cnt=113868800
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
    shm_unlink("/cnt");            /* removes a segment left by a crash */
    int fd = shm_open("/cnt", O_CREAT | O_RDWR, 0600);
    if (fd == -1) {
        handle_fatal_error("shm_open");
    }
    if (ftruncate(fd, sizeof(unsigned int)) == -1) {  /* set to zero */
        handle_fatal_error("ftruncate");
    }
    volatile unsigned int *cnt = mmap(NULL, sizeof *cnt,
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
        if (pid == 0) {                   /* child */
            for (int i = 0; i < NB_ITERS; i++) {
                (*cnt)++;                 /* shared */
            }
            exit(EXIT_SUCCESS);
        }
    }
    wait(NULL);
    wait(NULL);                           /* two children */
    printf("cnt=%u\n", *cnt);

    munmap((void *) cnt, sizeof *cnt);
    shm_unlink("/cnt");

    return EXIT_SUCCESS;
}
