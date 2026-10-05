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

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* strerror() */

/**
 * @file without_pb_reentrant.c
 * @see pb_reentrant.c
 *
 * A simple program to show the importance of using "reentrant" functions.
 *
 * f_r() is the reentrant version of f() (as rand_r() is for rand()): the
 * caller provides the state, so there is no hidden static variable any
 * more. Each thread has its own seed: no shared data, no mutex needed.
 * Same seed in every thread: each one displays the first value (16838).
 */

#define THREADS 4

int f_r(unsigned int *next) {
    *next = *next * 1103515245 + 12345;
    return (int) ((*next / 65536) % 32768);
}

void *doit(void *vargp) {
    (void) vargp;  /* Mark it as unused */
    unsigned int seed = 1;  /* private state, in the stack of the thread */
    printf("[%lu]: val = %d\n", (unsigned long) pthread_self(), f_r(&seed));
    return NULL;
}

int main(void) {
    int i;
    int err;
    pthread_t tid[THREADS];

    for (i = 0; i < THREADS; i++) {
        if ((err = pthread_create(&tid[i], NULL, doit, NULL)) != 0) {
            fprintf(stderr, "pthread_create : %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < THREADS; i++) {
        if ((err = pthread_join(tid[i], NULL)) != 0) {
            fprintf(stderr, "pthread_join : %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
