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
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* EXIT_SUCCESS */
#include <string.h>  /* strerror() */

/**
 * Stops the program if a pthread_*() call failed: these functions return an
 * error number (0 on success) and do not set errno.
 */
static void check(int err, const char *what) {
    if (err != 0) {
        fprintf(stderr, "%s : %s\n", what, strerror(err));
        exit(EXIT_FAILURE);
    }
}

/**
 * @file thread_arg_pitfall.c
 * @brief Pitfall: passing the address of a loop variable to several
 * threads, and its correction.
 *
 * Lecture: chapter os-08 « Threads » ("Exercice 1 – qu'affiche ce
 * programme ?" and its correction).
 *
 * Version 1 (wrong): the four threads receive the same address, that of i,
 * and each one reads i when it runs, while main() modifies it. The values
 * are unpredictable and may be duplicated, e.g. 2, 2, 4, 0.
 *
 * Version 2 (correct): one slot per thread, which no longer changes.
 *
 * Rule: never pass the address of a variable that changes afterwards.
 *
 * \code{.bash}
 *   $ ./thread_arg_pitfall
 *   version 1 (FAUX) : &i partagé
 *   thread 2
 *   thread 2
 *   thread 4
 *   thread 0
 *   version 2 : un argument par thread
 *   thread 0
 *   thread 1
 *   thread 3
 *   thread 2
 * \endcode
 */

void *f(void *arg) {
    printf("thread %d\n", *(int *) arg);
    return NULL;
}

int main(void) {
    pthread_t t[4];
    int i;

    printf("version 1 (FAUX) : &i partagé\n");
    for (i = 0; i < 4; i++) {
        check(pthread_create(&t[i], NULL, f, &i), "pthread_create");        /* WRONG */
    }
    for (i = 0; i < 4; i++) {
        check(pthread_join(t[i], NULL), "pthread_join");
    }

    printf("version 2 : un argument par thread\n");
    int ids[4];                  /* one argument per thread */
    for (int k = 0; k < 4; k++) {
        ids[k] = k;
        check(pthread_create(&t[k], NULL, f, &ids[k]), "pthread_create");
    }
    for (int k = 0; k < 4; k++) {
        check(pthread_join(t[k], NULL), "pthread_join");
    }

    return EXIT_SUCCESS;
}
