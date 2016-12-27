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
 * @file threads_01.c
 * 
 * A simple program using 2 POSIX threads.
 *
 * On Mac OS X, compile with gcc -Wall -Wextra -ansi -pedantic thread_01.c
 * On Linux, compile with gcc -Wall -Wextra -ansi -pedantic thread_01.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define _REENTRANT

int global_value = -10;

void *do_A(void *arg) {
	int i;
	int n;

	n = *(int *) arg;

	for (i = 0; i < n; i++)
		global_value = i;

	pthread_exit(NULL);
}

void *do_B(void *arg) {
	int i;
	int n;

	n = *(int *) arg;

	for (i = 0; i < n; i++)
		printf("%d\n", global_value);

	pthread_exit(NULL);
}

int main(void) {
	pthread_t thA;
	pthread_t thB;
	int n = 10;

	pthread_create(&thA, NULL, do_A, &n);
	pthread_create(&thB, NULL, do_B, &n);

	/* ... */
	sleep(1);

	/* The main thread is waiting for A and B to finish */
	printf("The main thread is waiting for A and B to finish\n");
	pthread_join(thB, NULL);
	pthread_join(thA, NULL);

	pthread_exit(NULL);

	exit(EXIT_SUCCESS); /* Unreachable code */
}
