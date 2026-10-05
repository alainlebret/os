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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* strerror() */
#include <pthread.h>
#include <unistd.h>

/**
 * @file thread_02_withmutex.c
 *
 * A simple program using 3 POSIX threads and a mutex: same as thread_02.c,
 * but each block of letters is displayed under the protection of the mutex,
 * so that the blocks of the different threads no longer interleave.
 *
 * Compile with gcc -Wall -Wextra -pedantic -std=c11 -pthread thread_02_withmutex.c
 */

#define ITERATIONS 100000

pthread_mutex_t mutex;

/**
 * Stops the program if a pthread_*() call failed: these functions return an
 * error number (0 on success) and do not set errno.
 */
static void check(int err, const char *what) {
    if (err != 0) {
        fprintf(stderr, "%s : %s\n", what, strerror(err));
        exit(EXIT_FAILURE);
    }
}

void display(int n, char letter) {
    int i;
    int j;

    for (j = 0; j < n; j++) {
        pthread_mutex_lock(&mutex);
        for (i = 0; i < ITERATIONS; i++) {
            printf("%c", letter);
        }
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
    }
}

void *threadA(void *unused) {
    (void) unused; /* Deactivate warning */
    display(100, 'A');
    printf("\n End of the thread A\n");
    fflush(stdout);

    pthread_exit(NULL);
}

void *threadC(void *unused) {
    (void) unused; /* Deactivate warning */
    display(150, 'C');

    printf("\n End of the thread C\n");
    fflush(stdout);

    pthread_exit(NULL);
}

void *threadB(void *unused) {
    (void) unused; /* Deactivate warning */
    pthread_t thC;

    check(pthread_create(&thC, NULL, threadC, NULL), "pthread_create");
    display(100, 'B');

    printf("\n Thread B is waiting for thread C\n");
    check(pthread_join(thC, NULL), "pthread_join");

    printf("\n End of the thread B\n");
    fflush(stdout);

    pthread_exit(NULL);
}

int main(void) {
    pthread_t thA;
    pthread_t thB;

    pthread_mutex_init(&mutex, NULL);
    printf(" Creation of the thread A\n");
    check(pthread_create(&thA, NULL, threadA, NULL), "pthread_create");
    printf(" Creation of the thread B\n");
    check(pthread_create(&thB, NULL, threadB, NULL), "pthread_create");
    sleep(1);

    /* The main thread is waiting for A and B to finish */
    printf("The main thread is waiting for A and B to finish\n");
    check(pthread_join(thA, NULL), "pthread_join");
    check(pthread_join(thB, NULL), "pthread_join");
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}
