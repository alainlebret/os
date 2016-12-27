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
 * @file signal_07.c
 *
 * A simple program that uses POSIX signals and handles the SIGUSR1 signal.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() */
#include <unistd.h> /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */
#include <signal.h> /* sigaction */

unsigned char nbCalls = 7;

/**
 * Handles the signal SIGUSR1 by decrementing \em nbCalls.
 * @param signal Number of the received signal.
 */
void handleSevenLifes(int signal) {
	nbCalls--;
	printf("Still have %d lifes.\n", (int)nbCalls);
}

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. Decrements the number of lifes before sending
 * the signal SIGUSR1 to his child.
 */
void manage_parent(pid_t child) {
	printf("Parent process (PID %d)\n", getpid());

	while (nbCalls-- != 0) {
		sleep(2);
		printf("Parent: sending SIGUSR1 to the child %d...\n", child);
		kill(child, SIGUSR1);
	}

	exit(EXIT_SUCCESS);
}

/**
 * Manages the child process. Configures the handler to react to the signal
 * SIGUSR1. Child exits when \em nbCalls reaches 0.
 */
void manage_child() {
	struct sigaction managingLifes;

	printf("Child process (PID %d)\n", getpid());

	managingLifes.sa_handler = &handleSevenLifes;
	sigaction(SIGUSR1, &managingLifes, NULL);

	while (nbCalls != 0)
		;

	exit(EXIT_SUCCESS);
}

int main(void) {
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		handle_fatal_error("Error using fork().\n");
	}
	if (pid > 0) {
		manage_parent(pid);
	} else {
		manage_child();
	}

	exit(EXIT_SUCCESS);
}
