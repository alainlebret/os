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
 * @file process_02.c
 * 
 * A simple program that clones a process using the \c fork() primitive and
 * shows the evolution of a variable in parent and child processes.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour exit() */
#include <unistd.h> /* pour fork() */
#include <sys/types.h> /* pour pid_t */
#include <sys/wait.h> /* pour wait() */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Manages the parent process by modifying value of the parameter.
 */
void manage_parent(int *parameter) {
	printf("Parent process (PID %d)\n", getpid());
	printf("Parent modifies its own variable...\n");
	*(parameter) = 10;
	wait(NULL);
}

/**
 * Manages the child process by modifying value of the parameter.
 */
void manage_child(int *parameter) {
	printf("Child process (PID %d)\n", getpid());
	printf("Child modifies its own variable...\n");
	*parameter = 20;
}

int main(void) {
	pid_t pid;
	int own_variable = 0;

	pid = fork();
	if (pid < 0) {
		handle_fatal_error("Error using fork().\n");
	}

	if (pid > 0) {
		manage_parent(&own_variable);
	} else {
		manage_child(&own_variable);
	}
	printf("\nPID %d has its own variable equals to: %d\n", getpid(), own_variable);

	exit(EXIT_SUCCESS);
}
