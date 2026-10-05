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
#include <unistd.h>
#include <pthread.h>

#include "pthread_barrier.h"

int pthread_barrier_init(pthread_barrier_t *barrier, const void *attr, unsigned count) {
    int status;

    (void) attr;  /* attributes are not supported */
    status = pthread_mutex_init(&barrier->mutex, NULL);
    if (status != 0) return status;
    status = pthread_cond_init(&barrier->cond, NULL);
    if (status != 0) {
        pthread_mutex_destroy(&barrier->mutex);
        return status;
    }
    barrier->count = count;
    barrier->waiting = 0;
    barrier->generation = 0;
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    unsigned generation = barrier->generation;
    barrier->waiting++;
    if (barrier->waiting < barrier->count) {
        /* while loop: spurious wakeups; the predicate is "barrier opened" */
        while (generation == barrier->generation) {
            pthread_cond_wait(&barrier->cond, &barrier->mutex);
        }
        pthread_mutex_unlock(&barrier->mutex);
        return 0;
    } else {
        barrier->waiting = 0; /* reset for next use */
        barrier->generation++;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return PTHREAD_BARRIER_SERIAL_THREAD;
    }
}

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
    pthread_cond_destroy(&barrier->cond);
    pthread_mutex_destroy(&barrier->mutex);
    return 0;
}
