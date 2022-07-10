/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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

#define THREAD_COUNT 10

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

/**
 * @file barrier.c
 *
 * A simple program to show pthread_barrier synchronization.
 *
 */

pthread_barrier_t barrier;

void *doit(void *id)
{
    int tid;
    int wait_sec;

    tid = *(int *) id;
    wait_sec = 1 + rand() % 5;

    printf("thread %d: wait for %d seconds.\n", tid, wait_sec);
    sleep(wait_sec);
    printf("thread %d: Ready...\n", tid);

    pthread_barrier_wait(&barrier);

    printf("thread %d: passed the barrier!\n", tid);
    return NULL;
}

int main(void)
{
    int i;
    int tid_args[THREAD_COUNT];
    pthread_t tid[THREAD_COUNT];

    srand(time(NULL));
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT + 1);

    for (i = 0; i < THREAD_COUNT; i++) {
        tid_args[i] = i;
        pthread_create(&tid[i], NULL, doit, &tid_args[i]);
    }

    printf("The main thread is ready.\n");

    pthread_barrier_wait(&barrier);

    printf("The main thread passed the barrier!\n");

    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_barrier_destroy(&barrier);

    exit(EXIT_SUCCESS);
}
