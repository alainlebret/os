/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0

typedef sem_t semaphore_t;

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file posix_semaphore.c
 *
 * A simple program used to implement a POSIX semaphore.
 */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

/**
 * Creates a POSIX semaphore and returns it.
 * @param name The name of the semaphore on the Unix system.
 * @return A pointer on the created POSIX semaphore.
 */
semaphore_t *create_and_open_semaphore(char *name)
{
    semaphore_t *sem = NULL;

    sem = sem_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED) {
        handle_fatal_error("Error [sem_open()]: ");
    }
    return sem;
}

/**
 * Opens an already created POSIX semaphore and returns it.
 * @param name The name of the semaphore on the Unix system.
 * @return A pointer on the POSIX semaphore.
 */
semaphore_t *open_semaphore(char *name)
{
    semaphore_t *sem = NULL;

    sem = sem_open(name, O_RDWR, S_IRUSR | S_IWUSR, 0);
    if (sem == SEM_FAILED) {
        sem_unlink(name); /* Try to unlink it */
        handle_fatal_error("Error [sem_unlink()]: ");
    }
    return sem;
}

/**
 * Destroys the specifier POSIX semaphore.
 * @param sem The identifier of the semaphore to destroy
 */
void destroy_semaphore(semaphore_t *sem, char *name)
{
    int r = 0;

    r = sem_close(sem);
    if (r < 0) {
        handle_fatal_error("Error [sem_close()]: ");
    }
    r = sem_unlink(name);
    if (r < 0) {
        perror("Error [sem_unlink()]: ");
    }
}

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void P(semaphore_t *sem)
{
    int r = 0;

    r = sem_wait(sem);
    if (r < 0) {
        handle_fatal_error("Error [P()]: ");
    }
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void V(semaphore_t *sem)
{
    int r = 0;

    r = sem_post(sem);
    if (r < 0) {
        handle_fatal_error("Error [V()]: ");
    }
}

int main(int argc, char *argv[])
{
    semaphore_t *sem = NULL;
    int loop = TRUE;
    char choice;

    sem = create_and_open_semaphore("/thesemaphore");

    while (loop) {
        printf("p, v, x, q ? ");
        if (scanf("%c", &choice) != 1)
            break;

        switch (toupper(choice)) {
            case 'P':
                P(sem);
                printf("P() -- Access the critical section\n");
                break;
            case 'V':
                V(sem);
                printf("OK.\n");
                break;
            case 'X':
                destroy_semaphore(sem, "/thesemaphore");
                printf("V() -- Leave the critical section\n");
                loop = FALSE;
                break;
            case 'Q':
                loop = FALSE;
                break;
            default:
                printf("?\n");
        }
    }

    exit(EXIT_SUCCESS);
}

