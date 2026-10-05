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
#include <pthread.h>    /* pthread_create(), pthread_attr_*(), pthread_exit() */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file thread_detached.c
 * @brief Creates a detached thread using the PTHREAD_CREATE_DETACHED
 * attribute.
 *
 * Lecture: chapter os-08 « Threads » ("Thread détaché – exemple").
 *
 * Key points:
 *   - the resources of a detached thread are freed automatically when it
 *     ends; pthread_join() is impossible;
 *   - the attribute object is only read at creation: it can be destroyed
 *     right after pthread_create();
 *   - main() ends with pthread_exit(), which lets worker() finish; a
 *     return would call exit() and kill it.
 *
 * \code{.bash}
 *   $ ./thread_detached
 *   Thread détaché >> Bonjour
 * \endcode
 */

void *worker(void *arg) {
    printf("Thread détaché >> %s\n", (char *) arg);
    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_attr_t attr;
    int err;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    err = pthread_create(&tid, &attr, worker, "Bonjour");
    if (err != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    pthread_attr_destroy(&attr);
    pthread_exit(NULL);     /* no join possible */
}
