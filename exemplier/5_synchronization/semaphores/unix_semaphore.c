/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Process synchronization" / "Synchronisation des processus"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Alain Lebret <alain.rene.lebret@gmail.com>
 * @author Michel Billaud <michel.billaud@labri.fr>
 * @version	1.1
 * @date 2011-12-01
 */

/**
 * @file ipc_semaphore.c
 * 
 * Example using a System V semaphore.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

typedef int semaphore_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char message[])
{
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Creates a System V semaphore and returns its identifier.
 * @return The created POSIX semaphore.
 */
semaphore_t create_semaphore(key_t key)
{
    semaphore_t sem;
    int r;

    sem = semget(key, 1, IPC_CREAT | 0666);
    if (sem < 0) {
        handle_fatal_error("Error trying to create semaphore\n");
    }
    r = semctl(sem, 0, SETVAL, 0);      /* initial value = 0 */
    if (r < 0) {
        handle_fatal_error("Error trying to initialize semaphore\n");
    }

    return sem;
}

/**
 * Destroy the specifier System V semaphore.
 * @param sem The identifier of the semaphore to destroy
 */
void destroy_semaphore(semaphore_t sem)
{
    if (semctl(sem, 0, IPC_RMID, 0) != 0)
        handle_fatal_error("Error trying to destroy semaphore\n");
}

/**
 * Modifies the value of the specified System V semaphore.
 * @param sem The identifier of the semaphore
 * @param new_value The new value to associate to the semaphore
 */
void modify_semaphore_value(semaphore_t sem, int new_value)
{
    struct sembuf sb[1];

    sb[0].sem_num = 0;
    sb[0].sem_op = new_value;
    sb[0].sem_flg = 0;

    if (semop(sem, sb, 1) != 0)
        handle_fatal_error("Error trying to modify semaphore's value\n");
}

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param sem Identifier of the semaphore.
 */
void P(semaphore_t sem)
{
    modify_semaphore_value(sem, -1);
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Indentifier of the semaphore.
 */
void V(semaphore_t sem)
{
    modify_semaphore_value(sem, 1);
}

int main(int argc, char *argv[])
{
    semaphore_t sem;
    key_t key;
    int loop = TRUE;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s cle\n", argv[0]);
        handle_fatal_error("Error: bad number of arguments.\n");
    }
    key = atoi(argv[1]);
    sem = create_semaphore(key);

    while (loop) {
        char choice;
        printf("p, v, x, q ? ");
        if (scanf("%c", &choice) != 1)
            break;

        switch (toupper(choice)) {
            case 'P':
                P(sem);
                printf("P() -- Now access the critical section\n");
                break;
            case 'V':
                V(sem);
                printf("V() -- Leave the critical section\n");
                break;
            case 'X':
                destroy_semaphore(sem);
                printf("Semaphore has been destroy\n");
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
