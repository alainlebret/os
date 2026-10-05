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
 * @file thread_with_shm_without_mutex.c
 *
 * A simple program that creates 4 producer threads and 1 consumer. They all
 * share a memory without using a mutex.
 *
 * WARNING: synchronization is deliberately missing. Several producers may
 * see BUFFER_SPACE at the same time and all increment the value, and the
 * compiler may even keep memory.state in a register: the behavior is
 * wrong (data race). See thread_with_shm_and_mutex.c. Ctrl-C to stop.
 *
 * Compile with gcc -Wall -Wextra -pedantic -std=c11 -pthread thread_with_shm_without_mutex.c
 * @date 2012-04-10
 */

#define BUFFER_SPACE 0
#define BUFFER_FULL  1

struct {
    int state; /* BUFFER_SPACE or BUFFER_FULL */
    int value;
} memory;

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
    while (1) {
        if (memory.state == BUFFER_FULL) {
            /* get the value */
            printf("%d\n", memory.value);
            memory.state = BUFFER_SPACE;
        }
        sleep(2);
    }
    /* Unreachable */
}

/**
 * Producer task that writes on the shared memory.
 */
void *produce(void *pv) {
    (void) pv; /* Deactivate warning */
    while (1) {
        if (memory.state == BUFFER_SPACE) {
            /* doing some stuff ... */
            memory.state = BUFFER_FULL;
            memory.value = memory.value + 1;
        }
        /* sleep(1); */
    }
    /* Unreachable */
}

int main(void) {
    pthread_t th1;
    pthread_t th2;
    pthread_t th3;
    pthread_t th4;
    pthread_t th5;

    /* Creation of the threads */
    check(pthread_create(&th1, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th2, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th3, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th4, NULL, produce, 0), "pthread_create");
    check(pthread_create(&th5, NULL, consume, 0), "pthread_create");

    printf("4 producers and 1 consumer have been created\n");

    /* The main thread is waiting for all threads to finish */
    check(pthread_join(th1, NULL), "pthread_join");
    check(pthread_join(th2, NULL), "pthread_join");
    check(pthread_join(th3, NULL), "pthread_join");
    check(pthread_join(th4, NULL), "pthread_join");
    check(pthread_join(th5, NULL), "pthread_join");

    return EXIT_SUCCESS;
}
