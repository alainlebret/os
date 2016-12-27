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
 * @file process_07.c
 *
 * A simple program about a process and its group.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* getpid() and getpgrp() */
#include <sys/types.h> /* pid_t */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. Parent is waiting for his child.
 */
void manage_parent() {
	pid_t child;
	int status;

	printf("Parent process: %d\n", getpid());
	printf("Parent group: %d\n", getpgrp());
	child = wait(&status);
	if (WIFEXITED(status)) {
		printf("%d : child %d has finished is work (code: %d)\n", getpid(), child,
				 WEXITSTATUS(status));
	}
}

/**
 * Manages the child process. Child is calling the exec function to execute the
 * \em gnuplot program.
 */
void manage_child() {
	printf("Child process: %d\n", getpid());
	printf("Child group: %d\n", getpgrp());
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