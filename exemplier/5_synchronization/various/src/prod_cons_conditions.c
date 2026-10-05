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
#include <pthread.h>    /* pthread_*, pthread_mutex_*, pthread_cond_* */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file prod_cons_conditions.c
 *
 * Producer / consumer with a circular buffer of N slots, a mutex and two
 * condition variables (non_plein, non_vide), between two threads.
 * Lecture: chapter os-10 « Synchronisation » ("Variante : mutex et
 * conditions").
 *
 * The producer waits while the buffer is full (nb == N), the consumer
 * while it is empty (nb == 0); each one signals the other after its
 * operation. Works under macOS, unlike the version with anonymous
 * semaphores (../../semaphores/src/prod_cons_3_semaphores.c).
 *
 * In the lecture both threads loop forever; here the producer deposits
 * NB_ITEMS values and the consumer removes as many, so that the program
 * ends.
 *
 * \code{.bash}
 *   $ ./prod_cons_conditions
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

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t non_plein = PTHREAD_COND_INITIALIZER;
pthread_cond_t non_vide  = PTHREAD_COND_INITIALIZER;
int nb = 0;                    /* occupied slots */

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
        pthread_mutex_lock(&m);
        while (nb == N) {
            pthread_cond_wait(&non_plein, &m);
        }
        deposer(i);
        nb++;
        pthread_cond_signal(&non_vide);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

void *consommateur(void *a) {
    (void) a;
    for (int k = 0; k < NB_ITEMS; k++) {
        pthread_mutex_lock(&m);
        while (nb == 0) {
            pthread_cond_wait(&non_vide, &m);
        }
        int x = retirer();
        nb--;
        pthread_cond_signal(&non_plein);
        pthread_mutex_unlock(&m);
        printf("%d\n", x);     /* outside the critical section */
    }
    return NULL;
}

int main(void) {
    pthread_t prod, cons;
    int err;

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

    return EXIT_SUCCESS;
}
