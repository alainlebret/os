/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * Chapter "Threads" / Chapitre "Threads"
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
 * @date 2012-04-10
 */

/**
 * @file fork_test.c
 * 
 * Test de fork() pour comparaison avec les threads.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h> /* necessary for wait */
#include <sys/wait.h> /* necessary for wait */

#define NB_FORKS 50000

/**
 * @file fork_test.c
 *
 * A simple program to test fork vs threads (see \c thread_test.c).
 *
 * Compile using gcc -Wall -Wextra fork_test.c
 */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

void do_little() {
	int i;

	i = 0;
	i = i + 2;

	exit(EXIT_SUCCESS);
}

int main(void) {
	int pid;
	int status = 0;
	int i;
	clock_t begin_time;
	clock_t end_time;
	double duration;

	begin_time = clock();

	for (i = 0; i < NB_FORKS; i++) {
		if ((pid = fork()) < 0) {
			handle_fatal_error("Error when trying to fork\n");
		} else if (pid == 0) {
			do_little();
		} else {
			waitpid(pid, &status, 0);
		}
	}

	end_time = clock();
	duration = (double) (end_time - begin_time) / CLOCKS_PER_SEC;
	printf("%2.1f seconds\n", duration);

	exit(EXIT_SUCCESS);
}  
