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
#include <stdatomic.h>  /* atomic_uint, atomic_fetch_add() */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file cnt_thread_atomic.c
 *
 * Running example, correction 3: the shared counter is a C11 atomic
 * variable.
 * Lecture: chapter os-10 « Synchronisation » ("Fil rouge – correction 3 :
 * atomique").
 *
 * atomic_fetch_add() is a single indivisible instruction (x86: lock add):
 * neither lock nor waiting queue. On an atomic_uint, cnt++ is atomic too.
 * It only suits a simple operation on one variable.
 * Failed version: cnt_thread.c.
 *
 * \code{.bash}
 *   $ ./cnt_thread_atomic
 *   cnt=200000000
 * \endcode
 */

#ifndef NB_ITERS
#define NB_ITERS 100000000
#endif

atomic_uint cnt = 0;

void *count(void *arg) {
    (void) arg;
    for (int i = 0; i < NB_ITERS; i++) {
        atomic_fetch_add(&cnt, 1);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    int err;

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
    printf("cnt=%u\n", atomic_load(&cnt));

    return EXIT_SUCCESS;
}
