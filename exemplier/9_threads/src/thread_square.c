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
#include <stdlib.h>     /* malloc(), free(), exit() */
#include <string.h>     /* strerror() */

/**
 * @file thread_square.c
 * @brief Passes an argument to a thread and gets its result back.
 *
 * Lecture: chapter os-08 « Threads » ("Passer un argument, récupérer un
 * résultat").
 *
 * The thread receives the address of a long, computes its square in a
 * block allocated on the heap and returns its address. The main thread
 * gets it back with pthread_join(), displays it, then frees it.
 *
 * Key points:
 *   - never return the address of a local variable: the stack of the
 *     thread disappears with it;
 *   - several arguments: pass the address of a structure.
 *
 * \code{.bash}
 *   $ ./thread_square
 *   49
 * \endcode
 */

void *carre(void *arg) {
    long n = *(long *) arg;          /* argument */
    long *res = malloc(sizeof *res);
    if (res == NULL) {
        return NULL;
    }
    *res = n * n;
    return res;                      /* on the heap */
}

int main(void) {
    pthread_t tid;
    long n = 7;
    void *ret;
    int err;

    err = pthread_create(&tid, NULL, carre, &n);
    if (err != 0) {
        fprintf(stderr, "pthread_create : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    pthread_join(tid, &ret);         /* ret = res */
    if (ret == NULL) {
        fprintf(stderr, "carre : malloc a échoué\n");
        exit(EXIT_FAILURE);
    }
    printf("%ld\n", *(long *) ret);  /* 49 */
    free(ret);

    return EXIT_SUCCESS;
}
