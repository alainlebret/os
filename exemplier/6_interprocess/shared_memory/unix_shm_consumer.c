/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * Chapter "Interprocess communication" / Chapitre "Communication interprocessus"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file shm_consumer.c
 * 
 * Consumer using an IPC/System V shared memory. The program displays content
 * on the shared memory. It can be stopped using C-C.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour exit() et execl()*/
#include <unistd.h> /* pour fork() */
#include <sys/types.h> /* pour pid_t et mkfifo() */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>

#define DURATION 2
#define CONTINUE 1
#define STOP 0

/** 
 * Structure to store the number of some integers and their sum.
 */
struct data {
	int nb;
	int total;
};

typedef struct data data_t;

int loop = CONTINUE;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void stop_loop(int signal) {
	loop = STOP;
}

int main(void) {
	key_t key;
	int id;
	data_t *shared_memory;
	struct sigaction action;

	key = ftok(getenv("HOME"), 'A');
	if (key == -1) {
		handle_fatal_error("Error using ftok()!\n");
	}

	id = shmget(key, sizeof(data_t), 0);
	if (id == -1) {
		switch (errno) {
			case ENOENT:
				handle_fatal_error("No existing segment!\n");
				break;
			default:
				handle_fatal_error("Error using shmget()!\n");
		}
	}

	shared_memory = (data_t *) shmat(id, NULL, SHM_R);
	if (shared_memory == NULL) {
		handle_fatal_error("Error using shmat()!\n");
	}

	loop = CONTINUE;

	action.sa_handler = stop_loop;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGINT, &action, NULL);

	while (loop) {
		sleep(DURATION);
		printf("The sum of %d integers equals %d\n", shared_memory->nb,
				 shared_memory->total);
	}

	printf("---\n");
	if (shmdt((char *) shared_memory) == -1) {
		handle_fatal_error("Error using shmdt()!\n");
	}

	exit(EXIT_SUCCESS);
}
