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
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <semaphore.h>

/**
 * @file posix_semaphore.c
 * @brief A simple program used to implement a POSIX semaphore.
 *
 * This program is an interactive demonstration of POSIX semaphore usage. It
 * allows the user to perform semaphore operations like wait (P), signal (V),
 * destroy the semaphore (X), or quit the program (Q).
 *
 * Run it in two terminals: both processes open the same named semaphore
 * (initial value 1), so the second P() blocks until the other one does V().
 */

#define SEM_PATH "/thesemaphore"

typedef sem_t semaphore_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Creates a POSIX semaphore and returns it.
 * @param name The name of the semaphore on the Unix system.
 * @return A pointer on the created POSIX semaphore.
 */
semaphore_t *create_and_open_semaphore(char *name) {
    semaphore_t *sem = NULL;

    /* No O_EXCL: a second process opens the existing semaphore */
    sem = sem_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED) {
        handle_fatal_error("Error [sem_open()]");
    }
    return sem;
}

/**
 * Destroys the specified POSIX semaphore.
 * @param sem The identifier of the semaphore to destroy
 */
void destroy_semaphore(semaphore_t *sem, char *name) {
    int r = 0;

    r = sem_close(sem);
    if (r < 0) {
        handle_fatal_error("Error [sem_close()]");
    }
    r = sem_unlink(name);
    if (r < 0) {
        perror("Error [sem_unlink()]");
    }
}

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void P(semaphore_t *sem) {
    int r = 0;

    r = sem_wait(sem);
    if (r < 0) {
        handle_fatal_error("Error [P()]");
    }
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void V(semaphore_t *sem) {
    int r = 0;

    r = sem_post(sem);
    if (r < 0) {
        handle_fatal_error("Error [V()]");
    }
}

int main(void) {
    semaphore_t *sem = create_and_open_semaphore(SEM_PATH);
    char choice;

    while (1) {
        printf("p, v, x, q ? ");
        fflush(stdout);
        if (scanf(" %c", &choice) != 1) /* " %c" skips spaces and newlines */
            break;

        switch (toupper((unsigned char) choice)) {
            case 'P':
                P(sem);
                printf("P() -- Access granted to critical section\n");
                break;
            case 'V':
                V(sem);
                printf("V() -- Released critical section\n");
                break;
            case 'X':
                destroy_semaphore(sem, SEM_PATH);
                printf("Semaphore destroyed.\n");
                return EXIT_SUCCESS; /* Exit after destruction */
            case 'Q':
                return EXIT_SUCCESS; /* Clean exit */
            default:
                printf("Invalid choice. Use 'p', 'v', 'x', or 'q'.\n");
        }
    }

    return EXIT_SUCCESS;
}

