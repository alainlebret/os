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
#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <fcntl.h>          /* for O_* constants */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */

/**
 * @file unix_shm.c
 *
 * Example using child processes sharing System V memory and POSIX semaphore.
 * Link with \c -lpthread.
 *
 * @author Alain Lebret
 * @version	1.0.2
 * @date 2016-11-01
 */

int shmid;         /* shared memory id */
int *shared_value; /* shared variable */
sem_t *sem;        /* sync semaphore */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

/**
 * Initializes the shared memory and semaphore's value.
 * @return The number of child processes
 */
int initialize(void) {
    unsigned int number_children; /* fork count */
    unsigned int sem_value;       /* semaphore value */
    key_t shmkey;                 /* shared memory key */

    /* initialize a shared variable in shared memory */
    shmkey = ftok("/dev/null", 5);  /* valid directory name and a number */
    printf("shmkey for shared value = %d\n", shmkey);

    shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT);
    if (shmid < 0) {  /* shared memory error check */
        handle_fatal_error("Error [shmget()]");
    }

    shared_value = (int *) shmat(shmid, NULL, 0); /* attach to memory */
    if (shared_value == (void *) -1) {
        handle_fatal_error("Error [shmat()]");
    }
    *shared_value = 0;
    printf("shared value=%d is allocated in shared memory.\n\n", *shared_value);

    printf("How many children do you want to fork:\n");
    if (scanf("%u", &number_children) != 1) {
        fprintf(stderr, "A positive integer is expected.\n");
        shmctl(shmid, IPC_RMID, NULL); /* do not leave the segment behind */
        exit(EXIT_FAILURE);
    }

    /* 1: mutual exclusion (one child at a time in the critical section);
       > 1: several children may modify the shared value at the same time */
    printf("Enter a semaphore value: ");
    if (scanf("%u", &sem_value) != 1) {
        fprintf(stderr, "A positive integer is expected.\n");
        shmctl(shmid, IPC_RMID, NULL); /* do not leave the segment behind */
        exit(EXIT_FAILURE);
    }

    /* initialize semaphores for shared processes */
    sem = sem_open("/pSem", O_CREAT | O_EXCL, 0600, sem_value);
    if (sem == SEM_FAILED) {
        handle_fatal_error("Error [sem_open()]");
    }
    /* name of semaphore is "/pSem", semaphore is reached using this name */
    sem_unlink("/pSem");
    /* unlink prevents the semaphore existing forever */
    /* if a crash occurs during the execution         */
    printf("semaphores initialized.\n\n");
    fflush(stdout);  /* before fork(): do not duplicate the stdio buffer */

    return number_children;
}

/**
 * Manages the parent process. Parent is waiting for his child.
 */
void manage_parent(int shmid) {
    printf("Parent process (PID %ld)\n", (long) getpid());

    /* wait for all children: wait() returns -1 (errno ECHILD) when none is left */
    while (wait(NULL) > 0) {
        ;
    }
    printf("\nParent: All children have exited.\n");

    /* shared memory detach */
    shmdt(shared_value);
    shmctl(shmid, IPC_RMID, 0);

    /* cleanup semaphores */
    if (sem_close(sem) == -1) {
        handle_fatal_error("Error closing semaphore");
    }
}

/**
 * Manages the child process. Child enters critical section and modifies the
 * shared value.
 */
void manage_child(int child_number) {
    printf("Child %d process (PID %ld)\n", child_number, (long) getpid());

    sem_wait(sem);           /* P operation */
    printf("Child %d is in critical section.\n", child_number);
    sleep(1);
    *shared_value +=
            child_number % 3;  /* increment by 0, 1 or 2 based on number */
    printf("Child %d: new value = %d.\n", child_number, *shared_value);
    sem_post(sem);           /* V operation */
    printf("Child %d gets out of critical section.\n", child_number);

    if (sem_close(sem) == -1) {
        handle_fatal_error("Error closing semaphore");
    }

}

int main(void) {
    pid_t pid = 1;       /* > 0: parent, even if no child is created */
    int number_children; /* fork count */
    int child_number;

    number_children = initialize();

    /* fork child processes */
    for (child_number = 0; child_number < number_children; child_number++) {
        pid = fork();
        if (pid < 0) {
            handle_fatal_error("Error [fork()]");
        } else if (pid == 0)
            break;  /* child processes */
    }

    if (pid > 0) {
        manage_parent(shmid);
    } else {
        manage_child(child_number);
    }

    return EXIT_SUCCESS;
}

