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
 * @file shm_producer.c
 *
 * Producer using an IPC/System V shared memory. The program reads a serie of
 * integers and stores their sum in a shared memory.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour exit() et execl()*/
#include <unistd.h> /* pour fork() */
#include <sys/types.h> /* pour pid_t et mkfifo() */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>

#define EVER ;;

/** 
 * Structure to store the number of some integers and their sum.
 */
struct data {
	int nb;
	int total;
};

typedef struct data data_t;

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(void) {
	key_t key;
	int id;
	struct data *shared_memory;
	int value;

	key = ftok(getenv("HOME"), 'A');
	if (key == -1) {
		handle_fatal_error("Error using ftok()!\n");
	}

	id = shmget(key, sizeof(data_t), IPC_CREAT | IPC_EXCL | 0666);
	if (id == -1) {
		switch (errno) {
			case EEXIST:
				handle_fatal_error("Segment already exists.\n");
				break;
			default:
				handle_fatal_error("Error using shmget() !\n");
		}
	}

	shared_memory = (data_t *) shmat(id, NULL, SHM_R | SHM_W);
	if (NULL == shared_memory) {
		handle_fatal_error("Error using shmat()!\n");
	}

	shared_memory->nb = 0;
	shared_memory->total = 0;

	for (EVER) {
		printf("+ ");
		if (scanf("%d", &value) != 1) {
			break;
		}
		shared_memory->nb++;
		shared_memory->total += value;
		printf("The sum of %d integers equals %d\n", shared_memory->nb,
				 shared_memory->total);
	}
	printf("---\n");

	if (shmdt((char *) shared_memory) == -1) {
		handle_fatal_error("Error using shmdt() !\n");
	}

	/* remove the memory segment */
	if (shmctl(id, IPC_RMID, NULL) == -1) {
		handle_fatal_error("Error using shmctl/remove !\n");
	}

	exit(EXIT_SUCCESS);
}
