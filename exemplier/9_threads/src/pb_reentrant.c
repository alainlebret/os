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
 * @file pb_reentrant.c
 * @see without_pb_reentrant.c
 *
 * A simple program to show the importance of using "reentrant" functions.
 *
 * f() is the example implementation of rand() given by the C standard: its
 * state (next) is a hidden static variable shared by all the threads. The
 * threads modify it concurrently (data race) and the sequence of values
 * depends on the scheduling. Correction: without_pb_reentrant.c.
 *
 * What to observe: the 4 threads display 4 different values (16838, 5758,
 * 10113, 17515, in an order that changes): they share one sequence, each
 * call moves the hidden state for everybody. With without_pb_reentrant.c,
 * every thread displays 16838: each one has its own sequence.
 */

#define THREADS 4


int f(void) {
    static unsigned int next = 1;

    next = next * 1103515245 + 12345;
    return (int) ((next / 65536) % 32768);
}

void *doit(void *vargp) {
    (void) vargp; /* Deactivate warning */
    printf("[%lu]: val = %d\n", (unsigned long) pthread_self(), f());
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
