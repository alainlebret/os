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
#define _POSIX_C_SOURCE 200809L
#include <pthread.h>    /* pthread_create(), pthread_join() */
#include <stdio.h>      /* printf(), fprintf() */
#include <stdlib.h>     /* malloc(), free(), exit(), rand_r(), RAND_MAX */
#include <string.h>     /* strerror() */
#include <time.h>       /* clock_gettime() */

/**
 * @file matrix_product_threads.c
 * @brief Running example: square matrix product R = A x B computed by
 * 4 threads, one per quadrant of R.
 *
 * Lecture: chapter os-08 « Threads » (running example "découpage en
 * quadrants").
 *
 * Each element R[i][j] is the dot product of row i of A and column j of B:
 * it is computed independently of the others. Each thread computes one
 * quadrant of R (0: top left, 1: top right, 2: bottom left, 3: bottom
 * right), so no two threads write to the same place: no lock is needed.
 *
 * The program multiplies two n x n matrices (n even, 512 by default or
 * given as argument) with one thread, then with 4 threads, checks that both
 * results are equal and displays both durations.
 *
 * Key points:
 *   - each thread receives the address of its own args_t structure;
 *   - args[] remains valid until the pthread_join() calls;
 *   - small matrices: the multi-thread version is slower, since creating
 *     and joining the threads costs more than the computation;
 *   - the matrices are filled with rand_r() and a private seed (see the
 *     slide "Correction : les versions _r").
 *
 * \code{.bash}
 *   $ ./matrix_product_threads 1024
 *   n = 1024 : 1 thread 2.84 s, 4 threads 0.93 s, résultats identiques
 * \endcode
 */

typedef struct matrix {
    int rows;
    int columns;
    double **matrix;
} matrix_t;

typedef struct args_t {  /* one per thread */
    int quadrant;        /* 0 to 3 */
    matrix_t *m1;        /* A */
    matrix_t *m2;        /* B */
    matrix_t *answer;    /* R = A x B */
} args_t;

void multi_thread(matrix_t *m1, matrix_t *m2, matrix_t *answer);
void *product_matrix_thread(void *args);

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Allocates an n x n matrix (contents not initialized).
 */
matrix_t *create_matrix(int n) {
    matrix_t *m = malloc(sizeof *m);
    if (m == NULL) {
        handle_fatal_error_and_exit("malloc");
    }
    m->rows = n;
    m->columns = n;
    m->matrix = malloc(n * sizeof(double *));
    if (m->matrix == NULL) {
        handle_fatal_error_and_exit("malloc");
    }
    for (int i = 0; i < n; i++) {
        m->matrix[i] = malloc(n * sizeof(double));
        if (m->matrix[i] == NULL) {
            handle_fatal_error_and_exit("malloc");
        }
    }
    return m;
}

/**
 * @brief Frees a matrix created by create_matrix().
 */
void free_matrix(matrix_t *m) {
    for (int i = 0; i < m->rows; i++) {
        free(m->matrix[i]);
    }
    free(m->matrix);
    free(m);
}

/**
 * @brief Fills a matrix with pseudo-random values in [0, 1).
 */
void fill_matrix(matrix_t *m, unsigned int graine) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->columns; j++) {
            m->matrix[i][j] = rand_r(&graine) / ((double) RAND_MAX + 1);
        }
    }
}

/**
 * @brief Computes the quadrant a->quadrant of R = A x B.
 */
void *product_matrix_thread(void *args) {
    args_t *a = args;
    double **A = a->m1->matrix, **B = a->m2->matrix;
    double **R = a->answer->matrix;
    int h = a->answer->rows / 2;     /* half size */
    int i0 = (a->quadrant / 2) * h;  /* first row */
    int j0 = (a->quadrant % 2) * h;  /* first column */

    for (int i = i0; i < i0 + h; i++) {
        for (int j = j0; j < j0 + h; j++) {
            R[i][j] = 0.0;
            for (int k = 0; k < a->m1->columns; k++) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

/**
 * @brief Computes R = A x B with 4 threads, one per quadrant.
 */
void multi_thread(matrix_t *m1, matrix_t *m2, matrix_t *answer) {
    pthread_t tid[4];
    args_t args[4];           /* one block per thread */
    int err;

    for (int q = 0; q < 4; q++) {
        args[q] = (args_t){ q, m1, m2, answer };
        err = pthread_create(&tid[q], NULL,
                             product_matrix_thread, &args[q]);
        if (err != 0) {
            fprintf(stderr, "pthread_create : %s\n", strerror(err));
            exit(EXIT_FAILURE);
        }
    }
    for (int q = 0; q < 4; q++) {
        pthread_join(tid[q], NULL);
    }
}

/**
 * @brief Computes R = A x B in the calling thread (reference version).
 */
void single_thread(matrix_t *m1, matrix_t *m2, matrix_t *answer) {
    for (int q = 0; q < 4; q++) {
        args_t a = { q, m1, m2, answer };
        product_matrix_thread(&a);
    }
}

/**
 * @brief Returns the current time in seconds (monotonic clock).
 */
double now(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char *argv[]) {
    int n = 512;
    double t0, t1, t2;
    int identical = 1;

    if (argc > 1) {
        char *end;
        n = (int) strtol(argv[1], &end, 10);
        if (*end != '\0') {
            n = 0;  /* not a number: rejected below */
        }
    }
    if (n <= 0 || n % 2 != 0) {
        fprintf(stderr, "usage : %s [n pair > 0]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    matrix_t *a = create_matrix(n);
    matrix_t *b = create_matrix(n);
    matrix_t *r1 = create_matrix(n);
    matrix_t *r4 = create_matrix(n);
    fill_matrix(a, 1);
    fill_matrix(b, 2);

    t0 = now();
    single_thread(a, b, r1);
    t1 = now();
    multi_thread(a, b, r4);
    t2 = now();

    for (int i = 0; i < n && identical; i++) {
        for (int j = 0; j < n; j++) {
            if (r1->matrix[i][j] != r4->matrix[i][j]) {
                identical = 0;
                break;
            }
        }
    }
    printf("n = %d : 1 thread %.2f s, 4 threads %.2f s, résultats %s\n",
           n, t1 - t0, t2 - t1, identical ? "identiques" : "DIFFÉRENTS");

    free_matrix(a);
    free_matrix(b);
    free_matrix(r1);
    free_matrix(r4);

    return EXIT_SUCCESS;
}
