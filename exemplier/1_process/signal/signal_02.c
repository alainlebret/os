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
 * @file signal_02.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */
#include <signal.h> /* sigaction() */

#define DURATION 5

/** 
 * New handler of the SIGALRM signal.
 * @param signal Number of the signal
 */
void handleAlarm(int signal) {
	printf("Too late!\n");

	exit(EXIT_FAILURE);
}

int main(void) {
	struct sigaction action;
	int value;
	int remaining_time;

	action.sa_handler = &handleAlarm;
	sigaction(SIGALRM, &action, NULL);
	printf("You have %d seconds to enter a number: ", DURATION);

	/* The OS will send an alarm signal to the process in 'DURATION' sec. */
	alarm(DURATION);

	/* Wait for the user to enter a value */
	scanf("%d", &value);

	/* Deactivate the sending of an alarm signal by the OS */
	remaining_time = alarm(0);

	printf("Elapsed time: %d seconds.\n", 5 - remaining_time);

	exit(EXIT_SUCCESS);
}
