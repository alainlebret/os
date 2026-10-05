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
 * @file prod_cons_3_semaphores.c
 *
 * Producer / consumer with a circular buffer of N slots and three anonymous
 * POSIX semaphores, between two threads.
 * Lecture: chapter os-10 « Synchronisation » ("Le tampon circulaire",
 * "Solution à trois sémaphores", "Producteur et consommateur").
 *
 *   - vides (N): free slots, blocks the producer when the buffer is full;
 *   - pleins (0): occupied slots, blocks the consumer when it is empty;
 *   - mutex (1): protects tampon, in and out.
 * Each thread takes a token "at home" and gives one back "at the other's".
 * The order of the P operations matters: counting semaphore first, then
 * mutex (the reverse leads to a deadlock when the buffer is full).
 *
 * In the lecture both threads loop forever; here the producer deposits
 * NB_ITEMS values and the consumer removes as many, so that the program
 * ends. See prod_cons_conditions.c (../../various/src) for the variant
 * with a mutex and condition variables.
 *
 * LINUX ONLY: sem_init() is not implemented under macOS (ENOSYS).
 * Compile with \c -pthread.
 *
 * \code{.bash}
 *   $ ./prod_cons_3_semaphores
 *   0
 *   1
 *   ...
 *   19
 * \endcode
 */

#define N 8
#define NB_ITEMS 20

int tampon[N];
int in = 0, out = 0;           /* circular indices */
sem_t vides, pleins, mutex;

void deposer(int x) {
    tampon[in] = x;
    in = (in + 1) % N;
}

int retirer(void) {
    int x = tampon[out];
    out = (out + 1) % N;
    return x;
}

void *producteur(void *a) {
    (void) a;
    for (int i = 0; i < NB_ITEMS; i++) {
        sem_wait(&vides);
        sem_wait(&mutex);
        deposer(i);
        sem_post(&mutex);
        sem_post(&pleins);
    }
    return NULL;
}

void *consommateur(void *a) {
    (void) a;
    for (int k = 0; k < NB_ITEMS; k++) {
        sem_wait(&pleins);
        sem_wait(&mutex);
        int x = retirer();
        sem_post(&mutex);
        sem_post(&vides);
        printf("%d\n", x);     /* outside the critical section */
    }
    return NULL;
}

int main(void) {
    pthread_t prod, cons;
    int err;

    if (sem_init(&vides, 0, N) == -1 ||      /* free slots */
        sem_init(&pleins, 0, 0) == -1 ||     /* occupied slots */
        sem_init(&mutex, 0, 1) == -1) {      /* access to the buffer */
        perror("sem_init (not supported on macOS)");
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_create(&prod, NULL, producteur, NULL)) != 0 ||
        (err = pthread_create(&cons, NULL, consommateur, NULL)) != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_join(prod, NULL)) != 0 ||
        (err = pthread_join(cons, NULL)) != 0) {
        fprintf(stderr, "pthread_join : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }

    sem_destroy(&vides);
    sem_destroy(&pleins);
    sem_destroy(&mutex);

    return EXIT_SUCCESS;
}
