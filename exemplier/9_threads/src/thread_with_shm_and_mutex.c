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
 * @file thread_with_shm_and_mutex.c
 *
 * A simple program that creates 4 producer threads and 1 consumer. They all
 * share a memory using a mutex. When the producers are finished, the main
 * thread sets producers_done and the consumer stops once the buffer is empty.
 *
 * Note: the threads test memory.state in a loop (active polling). A
 * condition variable avoids this wasted time: see thread_with_conditions.c.
 *
 * Compile with gcc -Wall -Wextra thread_with_shm_and_mutex.c -pthread
 */

#define BUFFER_SPACE 0
#define BUFFER_FULL 1
#define ITERATIONS 100000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct {
    int state; /* BUFFER_SPACE or BUFFER_FULL */
    int value;
} memory;

int producers_done = 0; /* protected by the mutex */

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

/**
 * Consumer task that reads the shared memory.
 */
void *consume(void *pv) {
    (void) pv; /* Deactivate warning */
    int finished = 0;

    while (!finished) {
        pthread_mutex_lock(&mutex);
        if (memory.state == BUFFER_FULL) {
            /* use the value */
            printf("%d\n", memory.value);
            memory.state = BUFFER_SPACE;
        } else if (producers_done) {
            finished = 1; /* buffer empty and nothing more will come */
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

/**
 * Producer task that writes on the shared memory.
 */
void *produce(void *pv) {
    (void) pv; /* Deactivate warning */
    int i = 0;

    while (i++ < ITERATIONS) {
        pthread_mutex_lock(&mutex);
        if (memory.state == BUFFER_SPACE) {
            /* doing some stuff */
            memory.state = BUFFER_FULL;
            memory.value = memory.value + 1;
        }
        pthread_mutex_unlock(&mutex);
        /* sleep(1); */
    }
    pthread_exit(NULL);
}

int main(void) {
    pthread_t th1, th2, th3, th4, th5;

    /* Creation of the threads */
    check(pthread_create(&th1, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th2, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th3, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th4, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th5, NULL, consume, 0), "pthread_create");

    /* The main thread waits for the producers, then tells the consumer */
    check(pthread_join(th1, NULL), "pthread_join");
    check(pthread_join(th2, NULL), "pthread_join");
    check(pthread_join(th3, NULL), "pthread_join");
    check(pthread_join(th4, NULL), "pthread_join");

    pthread_mutex_lock(&mutex);
    producers_done = 1;
    pthread_mutex_unlock(&mutex);

    check(pthread_join(th5, NULL), "pthread_join");

    return EXIT_SUCCESS;
}
