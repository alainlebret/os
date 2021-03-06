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
 * @version	1.2
 * @date 2011-12-01

/**
 * @file posix_prod_cons.c
 *
 * Producer-consumer program using a POSIX semaphore.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>

#define EVER ;;
#define BUFFER_SIZE 5
#define BUFFER_SPACE 0
#define BUFFER_USED 1
#define MEM_PATH "/theshm"
#define SEM_PATH "/thesemaphore"

typedef sem_t semaphore_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
	perror(msg);
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
		handle_fatal_error("Error trying to create semaphore\n");
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
	if (sem < 0) {
		sem_unlink(name);
		handle_fatal_error("Error trying to open semaphore\n");
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
		handle_fatal_error("Error trying to destroy semaphore\n");
	}
	r = sem_unlink(name);
	if (r < 0) {
		perror("Error trying to unlink semaphore\n");
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
		handle_fatal_error("Error with P() operation\n");
	}
}

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void V(semaphore_t *sem)
{
	int r = 0;

	sem_post(sem);
	if (r < 0) {
		handle_fatal_error("Error with V() operation\n");
	}
}

/**
 * Handles the SIGINT signal.
 * @param signal
 */
void handler(int signal)
{
	sem_unlink(SEM_PATH);
	shm_unlink(MEM_PATH);
	exit(EXIT_SUCCESS);
}

/**
 * Writes values to a shared buffer protected by a semaphore.
 * @param buffer Buffer to read on.
 */
void produce()
{
	int in_index;
	int next_value;
	int shmd;
	semaphore_t * sem;
	int *buffer;
	struct sigaction action;

	action.sa_handler = &handler;
	sigaction(SIGINT, &action, 0);

	srand((unsigned int) getpid());
	in_index = 0;

	shmd = shm_open(MEM_PATH, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (shmd < 0) {
		handle_fatal_error("Error when creating shared memory\n");
	}
	ftruncate(shmd, BUFFER_SIZE);
	buffer = (int *) mmap(NULL, BUFFER_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
			shmd, 0);
	if (buffer == MAP_FAILED) {
		handle_fatal_error("Error calling mmap\n");
	}

	sem = create_and_open_semaphore(SEM_PATH);


	for (EVER) { /* produce next_value */
		sleep((unsigned int) (3 + rand() % 3));
		next_value = rand() % 100;
		printf("producer[%d]: %d\n", in_index, next_value);

		P(sem);

		buffer[in_index] = next_value;
		in_index = (in_index + 1) % BUFFER_SIZE;

		V(sem);
	}
}

/**
 * Reads values from a shared buffer a shared buffer protected by a semaphore.
 * @param buffer Buffer to read on.
 * @param id_semaphore Semaphore identifier used to protect critical section.
 */
void consume()
{
	int out_index;
	int next_value;
	int shmd;
	semaphore_t * sem;
	int *buffer;
	struct sigaction action;

	action.sa_handler = &handler;
	sigaction(SIGINT, &action, 0);

	srand((unsigned int) getpid());
	out_index = 0;

	shmd = shm_open(MEM_PATH, O_RDWR, S_IRUSR|S_IWUSR);
	if (shmd < 0) {
		handle_fatal_error("Error when creating shared memory\n");
	}
	buffer = (int *) mmap(NULL, BUFFER_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,
			shmd, 0);
	if (buffer == MAP_FAILED) {
		handle_fatal_error("Error calling mmap\n");
	}

	sem = open_semaphore(SEM_PATH);

	for (EVER) { /* consume next_value */
		P(sem);

		next_value = buffer[out_index];
		out_index = (out_index + 1) % BUFFER_SIZE;

		V(sem);

		/* consume next */
		printf("consumer[%d]: %d\n", out_index == 0 ? 4 : out_index - 1, next_value);
		sleep((unsigned int) (5 + rand() % 5));
	}
}

int main(void)
{
	if (fork() == 0) {
		produce();
	}
	sleep(5);
	if (fork() == 0) {
		consume();
	}

	wait(NULL);

	exit(EXIT_SUCCESS);
}
