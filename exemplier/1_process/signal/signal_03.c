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
 * @file signal_03.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */
#include <signal.h> /* sigaction() */

#define EVER ;;

unsigned int h = 0; /* Hours */
unsigned int m = 0; /* Minutes */
unsigned int s = 0; /* Seconds */

/** 
 * New handler of the SIGALRM signal.
 * @param signal Number of the signal
 */
void tick(int signal) {
	s++;
	if (s == 60) {
		s = 0;
		m++;
		if (m == 60) {
			m = 0;
			h++;
			if (h == 24)
				h = 0;
		}
	}
	printf("%d:%d:%d\n", h, m, s);

	/* Re-engage the alarm */
	alarm(1);
}

int main(void) {
	struct sigaction action;

	action.sa_handler = &tick;
	sigaction(SIGALRM, &action, NULL);

	/* Ask the OS to send a SIGALRM signal every second */
	alarm(1);

	/* Waiting for SIGALRM signals */
	for (EVER)
		;

	exit(EXIT_SUCCESS); /* unreachable code */
}
