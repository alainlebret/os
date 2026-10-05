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
#include <pthread.h>    /* pthread_create(), pthread_join() */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file cnt_thread.c
 *
 * Running example, failed synchronization (thread version): two threads
 * increment a shared counter NB_ITERS times each, without protection.
 * Lecture: chapter os-10 « Synchronisation » ("Synchronisation ratée –
 * version thread"), also shown in chapter os-08 « Threads ».
 *
 * cnt should be 200 000 000, but the result is wrong and changes from one
 * execution to the next: cnt++ is three instructions (Load, Add, Store),
 * so updates get lost when the two threads interleave.
 *
 * The counter is declared volatile, as suggested in the lecture: otherwise
 * the optimizer (-O2) may merge the loop into a single cnt += NB_ITERS.
 * Corrections: cnt_thread_mutex.c, cnt_thread_atomic.c and
 * ../semaphores/src/cnt_thread_semaphore.c.
 *
 * \code{.bash}
 *   $ gcc -O0 -pthread -o cnt_thread cnt_thread.c
 *   $ ./cnt_thread
 *   cnt=94832671
 *   $ ./cnt_thread
 *   cnt=101488317
 * \endcode
 */

#ifndef NB_ITERS
#define NB_ITERS 100000000
#endif

volatile unsigned int cnt = 0;       /* shared */

void *count(void *arg) {
    (void) arg;
    for (int i = 0; i < NB_ITERS; i++) {
        cnt++;
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
