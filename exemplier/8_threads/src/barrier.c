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
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#ifdef __APPLE__
#include "pthread_barrier.h"
#endif

/**
 * @file barrier.c
 *
 * A simple program to show pthread_barrier synchronization.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

pthread_barrier_t barrier;

void *doit(void *arg)
{
    int tid = *(int *)arg;
    int wait_sec = 1 + rand() % 5;

    printf("thread %d: wait for %d seconds.\n", tid, wait_sec);
    sleep(wait_sec);
    printf("thread %d: Ready...\n", tid);

    pthread_barrier_wait(&barrier);

    printf("thread %d: passed the barrier!\n", tid);

    free(arg);  /* Free the dynamically allocated memory */
    return NULL;
}

int main(void)
{
    pthread_t tid[THREAD_COUNT];
    int i, status;

    srand(time(NULL));
    status = pthread_barrier_init(&barrier, NULL, THREAD_COUNT + 1);
    if (status != 0) {
        perror("pthread_barrier_init failed");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        int *arg = malloc(sizeof(int));  /* Dynamically allocate memory for thread ID */
        if (arg == NULL) {
            perror("Failed to allocate memory for thread ID");
            exit(EXIT_FAILURE);
        }
        *arg = i;
        status = pthread_create(&tid[i], NULL, doit, arg);
        if (status != 0) {
            perror("pthread_create failed");
            free(arg);  // Free the allocated memory in case of failure
            exit(EXIT_FAILURE);
        }
    }

    printf("The main thread is ready.\n");

    pthread_barrier_wait(&barrier);

    printf("The main thread passed the barrier!\n");

    for (i = 0; i < THREAD_COUNT; i++) {
        status = pthread_join(tid[i], NULL);
        if (status != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    pthread_barrier_destroy(&barrier);

    return EXIT_SUCCESS;
}
