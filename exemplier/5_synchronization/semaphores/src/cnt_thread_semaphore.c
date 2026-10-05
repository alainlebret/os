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
#include <pthread.h>    /* pthread_create(), pthread_join() */
#include <semaphore.h>  /* sem_init(), sem_wait(), sem_post(), sem_destroy() */
#include <stdio.h>      /* printf(), fprintf(), perror() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file cnt_thread_semaphore.c
 *
 * Running example, correction 1: the shared counter protected by an
 * anonymous POSIX semaphore initialized to 1 (P / V around cnt++).
 * Lecture: chapter os-10 « Synchronisation » ("Fil rouge – correction 1 :
 * sémaphore").
 *
 * pshared = 0: the semaphore is shared by the threads of the process.
 * Failed version: ../../various/src/cnt_thread.c.
 *
 * LINUX ONLY: sem_init() is not implemented under macOS (ENOSYS); there,
 * use a named semaphore (see cnt_fork_named_semaphore.c) or a mutex.
 * Compile with \c -pthread.
 *
 * \code{.bash}
 *   $ ./cnt_thread_semaphore
 *   cnt=200000000
 * \endcode
 */

#ifndef NB_ITERS
#define NB_ITERS 100000000
#endif

unsigned int cnt = 0;
sem_t sem;                     /* a single declaration */

void *count(void *arg) {
    (void) arg;
    for (int i = 0; i < NB_ITERS; i++) {
        sem_wait(&sem);        /* P */
        cnt++;                 /* critical section */
        sem_post(&sem);        /* V */
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    int err;

    if (sem_init(&sem, 0, 1) == -1) {   /* threads, value 1 */
        perror("sem_init (not supported on macOS)");
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_create(&t1, NULL, count, NULL)) != 0 ||
        (err = pthread_create(&t2, NULL, count, NULL)) != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_join(t1, NULL)) != 0 ||
        (err = pthread_join(t2, NULL)) != 0) {
        fprintf(stderr, "pthread_join : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    sem_destroy(&sem);             /* cnt is 200000000 */
    printf("cnt=%u\n", cnt);

    return EXIT_SUCCESS;
}
