/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <pthread.h>
#include <unistd.h>

/**
 * @file threads_01.c
 *
 * A simple program using 2 POSIX threads.
 *
 * On Mac OS X, compile with gcc -Wall -Wextra -ansi -pedantic thread_01.c
 * On Linux, compile with gcc -Wall -Wextra -ansi -pedantic thread_01.c -pthread
 */

int global_value = -10;

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

    pthread_create(&thA, NULL, do_A, &n);
    pthread_create(&thB, NULL, do_B, &n);

    /* ... */
    sleep(1);

    /* The main thread is waiting for A and B to finish */
    printf("The main thread is waiting for A and B to finish\n");
    pthread_join(thB, NULL);
    pthread_join(thA, NULL);

    return EXIT_SUCCESS;
}
