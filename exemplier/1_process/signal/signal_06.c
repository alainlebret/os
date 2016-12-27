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
 * @file signal_06.c
 *
 * A simple program that uses POSIX signals and manages masks to block signals.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */
#include <signal.h> /* sigaction */

#define NBR_SIGNALS 2

/** A group of signals to be blocked by the process */
int signals[NBR_SIGNALS] = { SIGINT, SIGTERM };

/** An array to store the old signal handlers */
struct sigaction old_handlers[NBR_SIGNALS];

int main(void) {
	sigset_t new_mask;
	sigset_t old_mask;
	sigset_t pending_signals;
	int i;

	/* 
	 * Create a mask to block the two signals.
	 */
	sigemptyset(&new_mask);
	for (i = 0; i < NBR_SIGNALS; i++) {
		sigaddset(&new_mask, signals[i]);
	}

	/* 
	 * Exchange old and new masks
	 */
	sigprocmask(SIG_SETMASK, &new_mask, &old_mask);

	/* 
	 * Sleep for 20 seconds (maybe enough to try sending CTRL-C and SIGTERM and
	 * SIGHUP signals)...
	 */
	printf("20 seconds to send <CTRL>-C and kill -15 %d to this process\n", getpid());
	sleep(20);
	printf("\nEnd of blocking \n");

	/* Get the list of pending signals */
	sigpending(&pending_signals);

	/* Decode pending signals */
	for (i = 1; i < NSIG; i++) {
		if (sigismember(&pending_signals, i)) {
			printf("Pending signal %d has been blocked.\n", i);
		}
	}

	exit(EXIT_SUCCESS);
}
