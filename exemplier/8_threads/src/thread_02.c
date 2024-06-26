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
 * @file threads_02.c
 *
 * A simple program using 3 POSIX threads.
 *
 * On Mac OS X, compile with gcc -Wall -Wextra -ansi -pedantic thread_02.c
 * On Linux, compile with gcc -Wall -Wextra -ansi -pedantic thread_02.c -pthread
 */

#define ITERATIONS 100000

void display(int n, char letter) {
    int i;
    int j;

    for (j = 1; j < n; j++) {
        for (i = 1; i < ITERATIONS; i++) {
            printf("%c", letter);
        }
        fflush(stdout);
    }
}

void *threadA(void *unused) {
    (void) unused; /* Deactivate warning */
    display(100, 65);
    printf("\n End of the thread A\n");
    fflush(stdout);

    pthread_exit(NULL);
}

void *threadC(void *unused) {
    (void) unused; /* Deactivate warning */
    display(150, 67);

    printf("\n End of the thread C\n");
    fflush(stdout);

    pthread_exit(NULL);
}

void *threadB(void *unused) {
    (void) unused; /* Deactivate warning */
    pthread_t thC;

    pthread_create(&thC, NULL, threadC, NULL);
    display(100, 66);

    printf("\n Thread B is waiting for thread C\n");
    pthread_join(thC, NULL);

    printf("\n End of the thread B\n");
    fflush(stdout);

    pthread_exit(NULL);
}

int main(void) {
    pthread_t thA;
    pthread_t thB;

    printf(" Creation of the thread A\n");
    pthread_create(&thA, NULL, threadA, NULL);
    printf(" Creation of the thread B\n");
    pthread_create(&thB, NULL, threadB, NULL);
    sleep(1);

    /* The main thread is waiting for A and B to finish */
    printf("The main thread is waiting for A and B to finish\n");
    pthread_join(thA, NULL);
    pthread_join(thB, NULL);

    return EXIT_SUCCESS;
}
