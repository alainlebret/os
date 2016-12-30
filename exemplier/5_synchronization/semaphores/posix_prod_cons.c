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
 * @author Alain Lebret (2011)
 * @author Janet Davis (2006)
 * @author Henry Walker (2004)
 * @version	1.2
 * @date 2011-12-01

/**
 * @file prod_cons.c
 *
 * Producer-consumer program using a POSIX semaphore.
 */

#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EVER ;;
#define BUFFER_SIZE 10
#define BUFFER_SPACE 0
#define BUFFER_USED 1

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Writes values to a shared buffer protected by a semaphore.
 * @param buffer Buffer to read on.
 * @param id_semaphore Semaphore identifier used to protect critical section.
 */
void produce(int *buffer, int id_semaphore) {
	int in_index;
	int next_value;
	struct sembuf sbuf;

	srand((unsigned int) getpid());
	sbuf.sem_flg = 0;
	in_index = 0;

	for (EVER) { /* produce next_value */
		sleep((unsigned int) (5 + rand() % 5));
		next_value = rand() % 100;
		printf("producer: %d\n", next_value);

		/* P() */
		sbuf.sem_num = BUFFER_SPACE;
		sbuf.sem_op = -1;
		semop(id_semaphore, &sbuf, 1);

		buffer[in_index] = next_value;
		in_index = (in_index + 1) % BUFFER_SIZE;

		/* V() */
		sbuf.sem_num = BUFFER_USED;
		sbuf.sem_op = 1;
		semop(id_semaphore, &sbuf, 1);
	}
}

/**
 * Reads values from a shared buffer a shared buffer protected by a semaphore.
 * @param buffer Buffer to read on.
 * @param id_semaphore Semaphore identifier used to protect critical section.
 */
void consume(int *buffer, int id_semaphore) {
	int out_index;
	int next_value;
	struct sembuf sbuf;

	srand((unsigned int) getpid());
	sbuf.sem_flg = 0;
	out_index = 0;

	for (EVER) { /* consume next_value */
		/* P() */
		sbuf.sem_num = BUFFER_USED;
		sbuf.sem_op = -1;
		semop(id_semaphore, &sbuf, 1);

		next_value = buffer[out_index];
		out_index = (out_index + 1) % BUFFER_SIZE;

		/* V() */
		sbuf.sem_num = BUFFER_SPACE;
		sbuf.sem_op = 1;
		semop(id_semaphore, &sbuf, 1);

		/* consume next */
		printf("consumer: %d\n", next_value);
		sleep((unsigned int) (10 + rand() % 10));
	}
}

void handler(int signal) {
}

int main(void) {
	int *buffer;
	int id_semaphore;
	int id_memory;

	/* creation of the set of 2 IPC semaphores */
	id_semaphore = semget(IPC_PRIVATE, 2, 0644 | IPC_CREAT | IPC_EXCL);
	if (id_semaphore == -1) {
		handle_fatal_error("Error creating the set of semaphores\n");
	}

	/* initialization of the two semaphores */
	semctl(id_semaphore, BUFFER_USED, SETVAL, 0);
	semctl(id_semaphore, BUFFER_SPACE, SETVAL, BUFFER_SIZE);

	/* creation and attachment of a shared memory */
	id_memory = shmget(IPC_PRIVATE, BUFFER_SIZE * sizeof(int),
								0644 | IPC_CREAT | IPC_EXCL);
	if (id_memory == -1) {
		semctl(id_semaphore, 0, IPC_RMID);
		handle_fatal_error("Error creating the shared memory\n");
	}
	buffer = shmat(id_memory, NULL, 0);

	if (fork() == 0) produce(buffer, id_semaphore);
	if (fork() == 0) consume(buffer, id_semaphore);

	/* remove IPC semaphores */
	signal(SIGINT, handler);
	pause();
	semctl(id_semaphore, 0, IPC_RMID);
	shmctl(id_memory, IPC_RMID, NULL);

	exit(EXIT_SUCCESS);
}
