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
#include <pthread.h>    /* pthread_create(), pthread_join(), pthread_self() */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* exit() */
#include <string.h>     /* strerror() */

/**
 * @file thread_hello.c
 * @brief First Pthread program: creates one thread and waits for it.
 *
 * Lecture: chapter os-08 « Threads » ("Premier programme").
 *
 * The main thread creates a secondary thread, displays both identifiers,
 * then waits for the secondary thread with pthread_join().
 *
 * Key points:
 *   - the secondary thread retrieves with pthread_self() the identifier
 *     stored in tid1;
 *   - the order of the lines is not guaranteed;
 *   - converting a pthread_t to unsigned long is not portable (debugging
 *     only);
 *   - Pthread functions return an error code and do not set errno.
 *
 * \code{.bash}
 *   $ gcc -Wall -pthread -o thread_hello thread_hello.c
 *   $ ./thread_hello
 *   principal : 140245863241536
 *   secondaire : 140245854848768
 *   Bonjour de la part de 140245854848768
 * \endcode
 */

void *todo(void *arg) {
    (void) arg;
    printf("Bonjour de la part de %lu\n",
           (unsigned long) pthread_self());
    return NULL;              /* = pthread_exit(NULL) */
}

int main(void) {
    pthread_t tid1;
    int err;

    err = pthread_create(&tid1, NULL, todo, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("principal : %lu\nsecondaire : %lu\n",
           (unsigned long) pthread_self(),
           (unsigned long) tid1);
    pthread_join(tid1, NULL);

    return EXIT_SUCCESS;
}
