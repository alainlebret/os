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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NB_THREADS 50000
#define OK 0

/**
 * @file test_pthread.c
 *
 * A simple program to test threads vs the fork one (see \c test_fork.c).
 * The elapsed (wall-clock) time is measured with clock_gettime().
 *
 * Compile using gcc -Wall -Wextra test_pthread.c -o test_pthread -pthread
 */

void *do_little(void *unused) {
    (void) unused; /* Deactivate warning */
    int i;

    i = 0;
    i = i + 2;

    pthread_exit(NULL);
}

int main(void) {
    int action, i;
    pthread_t tid;
    pthread_attr_t attr;
    struct timespec begin_time;
    struct timespec end_time;
    double duration;

    clock_gettime(CLOCK_MONOTONIC, &begin_time);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < NB_THREADS; i++) {
        action = pthread_create(&tid, &attr, do_little, NULL);
        if (action != OK) {
            fprintf(stderr, "pthread_create: %s\n", strerror(action));
            exit(EXIT_FAILURE);
        }

        /* Attente du thread */
        action = pthread_join(tid, NULL);
        if (action != OK) {
            fprintf(stderr, "pthread_join: %s\n", strerror(action));
            exit(EXIT_FAILURE);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    duration = (end_time.tv_sec - begin_time.tv_sec)
               + (end_time.tv_nsec - begin_time.tv_nsec) / 1e9;
    printf("%2.1f seconds\n", duration);

    pthread_attr_destroy(&attr);

    return EXIT_SUCCESS;
}
