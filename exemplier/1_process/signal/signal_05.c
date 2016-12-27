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
 * @file signal_05.c
 *
 * A simple program that uses POSIX signals and handles many signals.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */
#include <signal.h> /* sigaction */

#define EVER ;;

/**
 * A naive handler to display the received signal number.
 * @param signal Number of the signal.
 */
void handle(int signal) {
	printf("Signal number %d has been received.\n", signal);
	fflush(stdout);
}

int main(void) {
	struct sigaction action;

	action.sa_handler = &handle;
	action.sa_flags = 0;

	/* We do no block any specific signal */
	sigemptyset(&action.sa_mask);

	/* Three signals will be handled by the process */
	sigaction(SIGINT, &action, 0);
	sigaction(SIGQUIT, &action, 0);
	sigaction(SIGTERM, &action, 0);

	for (EVER)
		;

	exit(EXIT_SUCCESS); /* unreachable code */
}
