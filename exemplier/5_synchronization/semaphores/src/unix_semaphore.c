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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/**
 * @file unix_semaphore.c
 *
 * Example using a System V semaphore.
 *
 * This program is an interactive demonstration of System V semaphore usage. 
 * It allows the user to perform semaphore operations like wait (P), signal
 * (V), destroy the semaphore (X), or quit the program (Q).
 * Run it in two terminals with the same key: the first one creates the
 * semaphore with the value 0, the second one opens it without resetting it.
 *
 * @author Alain Lebret
 * @author Michel Billaud <michel.billaud@labri.fr>
 * @version	1.1
 * @date 2011-12-01
 */

#define TRUE 1
#define FALSE 0

typedef int semaphore_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Creates (or opens) a System V semaphore and returns its identifier.
 * @return The identifier of the System V semaphore.
 */
semaphore_t create_semaphore(key_t key) {
    semaphore_t sem;
    int r;

    sem = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (sem < 0) {                      /* already created: just open it */
        sem = semget(key, 1, 0600);
        if (sem < 0) {
            handle_fatal_error("Error [semget()]");
        }
        return sem;
    }
    /* POSIX passes the 4th argument of semctl(SETVAL) as a union semun
       (field val), which the program must define itself on Linux; passing
       an int works on the usual ABIs (Linux, macOS), as here. */
    r = semctl(sem, 0, SETVAL, 0);      /* initial value = 0 */
    if (r < 0) {
        handle_fatal_error("Error [semctl()]");
    }

    return sem;
}

/**
 * Destroys the specified System V semaphore.
 * @param sem The identifier of the semaphore to destroy
 */
void destroy_semaphore(semaphore_t sem) {
    if (semctl(sem, 0, IPC_RMID, 0) != 0)
        handle_fatal_error("Error [semctl()]");
}

/**
 * Modifies the value of the specified System V semaphore.
 * @param sem The identifier of the semaphore
 * @param new_value The new value to associate to the semaphore
 */
void modify_semaphore_value(semaphore_t sem, int new_value) {
    struct sembuf sb[1];

    sb[0].sem_num = 0;
    sb[0].sem_op = new_value;
    sb[0].sem_flg = 0;

    if (semop(sem, sb, 1) != 0)
        handle_fatal_error("Error [semop()]");
}

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param sem Identifier of the semaphore.
 */
void P(semaphore_t sem) {
    modify_semaphore_value(sem, -1);
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Indentifier of the semaphore.
 */
void V(semaphore_t sem) {
    modify_semaphore_value(sem, 1);
}

int main(int argc, char *argv[]) {
    semaphore_t sem;
    key_t key;
    char choice;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s key\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    key = atoi(argv[1]);
    sem = create_semaphore(key);

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
                destroy_semaphore(sem);
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

