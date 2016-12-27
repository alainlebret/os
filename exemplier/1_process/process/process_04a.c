/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Processes & signals" / "Processus et signaux"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file process_04a.c
 *
 * A simple program that clones a process using the \c fork() primitive, but
 * without waiting child process, which then becomes a zombie!
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() */
#include <unistd.h> /* fork() and sleep() */
#include <sys/types.h> /* pid_t */

#define EVER ;;
#define DURATION 5

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. Parent is not waiting for his child to die.
 */
void manage_parent() {
	printf("Parent process (PID %d)\n", getpid());
	printf("Parent will never wait for his child to finish\n");

	for (EVER)
		;
}

/**
 * Manages the child process. Child is blocked during \em DURATION seconds.
 */
void manage_child() {
	printf("Child process (PID %d)\n", getpid());
	printf("Child will be blocked during %d seconds...\n", DURATION);

	sleep(DURATION);

	printf("Child has finished to sleep.\n");
}

int main(void) {
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		handle_fatal_error("Error using fork().\n");
	}

	if (pid > 0) {
		manage_parent();
	} else {
		manage_child();
	}

	exit(EXIT_SUCCESS);
}
