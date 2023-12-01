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
 * @file shm_with_mutex.c
 *
 * A simple program that creates 4 producer threads and 1 consumer. They all
 * share a memory using a mutex.
 *
 * On Mac OS X, compile with gcc -DMUTEX -Wall -Wextra shm_without_mutex.c
 * On Linux, compile with gcc -Wall -Wextra shm_without_mutex.c -DMUTEX -pthread
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

#define BUFFER_SPACE 0
#define BUFFER_FULL 1
#define ITERATIONS 100000
#define FOREVER for(;;)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct {
    int status;
    int value;
} memory;

/**
 * Consumer task that reads the shared memory.
 */
void *consume(void *pv)
{
    FOREVER {
        pthread_mutex_lock(&mutex);

        if (memory.status == BUFFER_FULL) {
            /* utiliser la valeur */
            printf("%d\n", memory.value);
            memory.status = BUFFER_SPACE;
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    /* Unreachable */
}

/**
 * Producer task that writes on the shared memory.
 */
void *produce(void *pv)
{
    int i = 0;

    while (i++ < ITERATIONS) {
        pthread_mutex_lock(&mutex);
        if (memory.status == BUFFER_SPACE) {
            /* doing some stuff */
            memory.status = BUFFER_FULL;
            memory.value = memory.value + 1;
        }
        pthread_mutex_unlock(&mutex);
        /* sleep(1); */
    }
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t th1, th2, th3, th4, th5;

    /* Creation of the threads */
    pthread_create(&th1, NULL, produce, 0);
    pthread_create(&th2, NULL, produce, 0);
    pthread_create(&th3, NULL, produce, 0);
    pthread_create(&th4, NULL, produce, 0);
    pthread_create(&th5, NULL, consume, 0);

    /* The main thread is waiting for all threads to finish */
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);
    pthread_join(th5, NULL);

    return EXIT_SUCCESS;
}
