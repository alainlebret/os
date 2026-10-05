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
#include <pthread.h>    /* pthread_create(), pthread_join(), pthread_mutex_* */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file cnt_thread_mutex.c
 *
 * Running example, correction 2: the shared counter protected by a mutex.
 * Lecture: chapter os-10 « Synchronisation » ("Fil rouge – correction 2 :
 * mutex"), also shown in chapter os-08 « Threads » ("Vers le chapitre
 * Synchronisation").
 *
 * The sequence Load, Add, Store of cnt++ becomes indivisible: the result is
 * 200 000 000, but the program is slower since the threads wait for each
 * other. The thread that locks is the one that unlocks: the mutex is the
 * natural tool. main() is unchanged: no dynamic initialization is needed.
 * Failed version: cnt_thread.c.
 *
 * \code{.bash}
 *   $ ./cnt_thread_mutex
 *   cnt=200000000
 * \endcode
 */

#ifndef NB_ITERS
#define NB_ITERS 100000000
#endif

unsigned int cnt = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *count(void *arg) {
    (void) arg;
    for (int i = 0; i < NB_ITERS; i++) {
        pthread_mutex_lock(&m);
        cnt++;                 /* critical section */
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    int err;

    if ((err = pthread_create(&t1, NULL, count, NULL)) != 0 ||
        (err = pthread_create(&t2, NULL, count, NULL)) != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_join(t1, NULL)) != 0 ||
        (err = pthread_join(t2, NULL)) != 0) {
        fprintf(stderr, "pthread_join : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("cnt=%u\n", cnt);

    return EXIT_SUCCESS;
}
