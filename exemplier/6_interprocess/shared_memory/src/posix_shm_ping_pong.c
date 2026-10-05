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
#include <unistd.h>     /* ftruncate(), close(), fork() */
#include <semaphore.h>  /* sem_init(), sem_wait(), sem_post(), sem_destroy() */
#include <sys/mman.h>   /* shm_open(), mmap(), munmap(), shm_unlink() */
#include <sys/wait.h>   /* wait() */

/**
 * @file posix_shm_ping_pong.c
 *
 * Complete example of the lecture: a parent and a child play ping-pong
 * with a counter placed in a shared memory segment, synchronized by two
 * anonymous POSIX semaphores placed in the segment itself.
 * Lecture: chapter os-09b « Mémoire partagée » ("Exemple complet –
 * ping-pong").
 *
 * The child displays 0, 2, 4 and the parent 1, 3, 5. The semaphores are
 * initialized with pshared = 1, which requires them to be in shared memory.
 *
 * LINUX ONLY: sem_init() is not implemented under macOS (it fails with
 * ENOSYS); there, use named semaphores (sem_open()).
 * Link with \c -lrt -pthread under Linux.
 *
 * \code{.bash}
 *   $ ./posix_shm_ping_pong
 *   enfant : 0
 *   parent : 1
 *   enfant : 2
 *   parent : 3
 *   enfant : 4
 *   parent : 5
 * \endcode
 */

#define NOM "/pingpong"
#define N   3

typedef struct { sem_t enfant, parent; int val; } shm_t;

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
    if (ftruncate(fd, sizeof(shm_t)) == -1) {
        handle_error("Error [ftruncate()]");
    }
    shm_t *s = mmap(NULL, sizeof *s,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);
    if (s == MAP_FAILED) {
        handle_error("Error [mmap()]");
    }
    close(fd);

    if (sem_init(&s->enfant, 1, 0) == -1 || sem_init(&s->parent, 1, 0) == -1) {
        perror("Error [sem_init()] (not supported on macOS)"); /* before errno changes */
        munmap(s, sizeof *s);
        shm_unlink(NOM);
        exit(EXIT_FAILURE);
    }
    s->val = 0;

    pid_t pid = fork();
    if (pid == -1) {
        handle_error("Error [fork()]");
    }
    if (pid == 0) {                    /* child */
        for (int i = 0; i < N; i++) {
            sem_wait(&s->enfant);
            printf("enfant : %d\n", s->val++);
            fflush(stdout);
            sem_post(&s->parent);
        }
        return EXIT_SUCCESS;
    }
    for (int i = 0; i < N; i++) {      /* parent */
        sem_post(&s->enfant);
        sem_wait(&s->parent);
        printf("parent : %d\n", s->val++);
        fflush(stdout);
    }
    wait(NULL);

    sem_destroy(&s->enfant);
    sem_destroy(&s->parent);
    munmap(s, sizeof *s);
    shm_unlink(NOM);

    return EXIT_SUCCESS;
}
