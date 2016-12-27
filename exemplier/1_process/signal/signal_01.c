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
 * @file signal_01.c
 *
 * A simple program that uses POSIX signals and handles SIGINT.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <signal.h> /* sigaction() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */


/** 
 * New handler of the SIGINT signal.
 * @param signal Number of the signal
 */
void handle(int signal) {
	printf("SIGINT signal received!\n");

	exit(EXIT_SUCCESS);
}

int main(void) {
	struct sigaction action;
	action.sa_handler = &handle;

	/* install the new handler of the SIGINT signal */
	sigaction(SIGINT, &action, NULL);

	/* wait for the signal */
	pause();
	printf("This will never be displayed.\n");

	exit(EXIT_SUCCESS);
}
