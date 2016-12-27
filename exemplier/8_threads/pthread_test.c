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
 * @file pthread_test.c
 * 
 * A simple program to test threads vs the fork one (see \c fork_test.c).
 *
 * On Mac OS X, compile using gcc -Wall -Wextra pthread_test.c
 * On Linux, compile using gcc -Wall -Wextra pthread_test.c -lpthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define NB_THREADS 50000
#define OK 0

void *do_little(void * unused) {
	int i;

	i = 0;
	i = i + 2;

	pthread_exit(NULL);
}

int main() {
	int action, i;
	pthread_t tid;
	pthread_attr_t attr;
	clock_t begin_time;
	clock_t end_time;
	double duration;

	begin_time = clock();
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < NB_THREADS; i++) {
		action = pthread_create(&tid, &attr, do_little, NULL);
		if (action != OK) {
			printf("Error using pthread_create() : %d).\n", action);
			exit(EXIT_FAILURE);
		}

		/* Attente du thread */
		action = pthread_join(tid, NULL);
		if (action != OK) {
			printf("Error using pthread_join() : %d).\n", action);
			exit(EXIT_FAILURE);
		}
	}
	end_time = clock();
	duration = (double) (end_time - begin_time) / CLOCKS_PER_SEC;
	printf("%2.1f seconds\n", duration);

	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

	exit(EXIT_SUCCESS); /* unreachable code */
}
