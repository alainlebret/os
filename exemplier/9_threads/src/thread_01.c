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
 * @file thread_01.c
 *
 * A simple program using 2 POSIX threads.
 *
 * WARNING: synchronization is deliberately missing. Thread A writes
 * global_value while thread B reads it: the displayed values are
 * unpredictable (a data race). The sleep(1) in main() does not synchronize
 * anything: only pthread_join() guarantees that the threads are finished.
 *
 * Compile with gcc -Wall -Wextra -pedantic -std=c11 -pthread thread_01.c
 */

int global_value = -10;

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

void *do_A(void *arg) {
    int i;
    int n;

    n = *(int *) arg;

    for (i = 0; i < n; i++) {
        global_value = i;
    }

    pthread_exit(NULL);
}

void *do_B(void *arg) {
    int i;
    int n;

    n = *(int *) arg;

    for (i = 0; i < n; i++) {
        printf("%d\n", global_value);
    }
    pthread_exit(NULL);
}

int main(void) {
    pthread_t thA;
    pthread_t thB;
    int n = 10;

    check(pthread_create(&thA, NULL, do_A, &n), "pthread_create");
    check(pthread_create(&thB, NULL, do_B, &n), "pthread_create");

    /* ... */
    sleep(1);

    /* The main thread is waiting for A and B to finish */
    printf("The main thread is waiting for A and B to finish\n");
    check(pthread_join(thB, NULL), "pthread_join");
    check(pthread_join(thA, NULL), "pthread_join");

    return EXIT_SUCCESS;
}
