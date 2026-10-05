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

/**
 * @file thread_shared_variables.c
 * @brief Program to analyse: which variables are shared by the threads?
 *
 * Lecture: chapter os-08 « Threads » ("Exemple – un programme à analyser").
 *
 * | Variable              | Instances | Location              | Shared?       |
 * |-----------------------|-----------|-----------------------|---------------|
 * | ptr (global)          | 1         | data segment          | yes           |
 * | svar (local static)   | 1         | data segment          | yes           |
 * | msg (local to main)   | 1         | stack of main thread  | yes, via ptr  |
 * | i, tid (local to main)| 1         | stack of main thread  | no            |
 * | myid (local to todo)  | 2         | one in each stack     | no            |
 *
 * Key points:
 *   - (void *)i passes the value of i, not its address;
 *   - the secondary threads read msg, stored in the stack of the main
 *     thread, through the global pointer ptr;
 *   - ++svar is an unprotected concurrent access.
 *
 * GCC (>= 12) warns "storing the address of local variable 'msg' in 'ptr'":
 * expected, it is the point of the example; msg stays valid because main()
 * joins the threads before returning.
 *
 * \code{.bash}
 *   $ ./thread_shared_variables
 *   [0] : P1 (svar=1)
 *   [1] : P2 (svar=2)
 * \endcode
 */

char **ptr;                           /* global */

void *todo(void *vargp) {
    long myid = (long) vargp;         /* local */
    static int svar = 0;              /* static */

    printf("[%ld] : %s (svar=%d)\n", myid, ptr[myid], ++svar);
    return NULL;
}

int main(void) {
    pthread_t tid[2];
    char *msg[2] = { "P1", "P2" };    /* local */

    ptr = msg;
    for (long i = 0; i < 2; i++) {
        pthread_create(&tid[i], NULL, todo, (void *) i);
    }
    for (long i = 0; i < 2; i++) {
        pthread_join(tid[i], NULL);
    }
    return EXIT_SUCCESS;
}
