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
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

/**
 * @file threads_02.c
 *
 * A simple program using 3 POSIX threads.
 *
 * On Mac OS X, compile with gcc -Wall -Wextra -ansi -pedantic thread_02.c
 * On Linux, compile with gcc -Wall -Wextra -ansi -pedantic thread_02.c -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define _REENTRANT
#define ITERATIONS 100000

void display(int n, char lettre) {
	int i, j;

	for (j = 1; j < n; j++) {
		for (i = 1; i < ITERATIONS; i++)
			;
		printf("%c", lettre);
		fflush(stdout);
	}
}

void *threadA(void *unused) {
	display(100, 65);
	printf("\n End of the thread A\n");
	fflush(stdout);

	pthread_exit(NULL);
}

void *threadC(void *unused) {
	display(150, 67);

	printf("\n End of the thread C\n");
	fflush(stdout);

	pthread_exit(NULL);
}

void *threadB(void *unused) {
	pthread_t thC;

	pthread_create(&thC, NULL, threadC, NULL);
	display(100, 66);

	printf("\n Thread B is waiting for thread C\n");
	pthread_join(thC, NULL);

	printf("\n End of the thread B\n");
	fflush(stdout);

	pthread_exit(NULL);
}

int main(void) {
	pthread_t thA, thB;

	printf(" Creation of the thread A\n");
	pthread_create(&thA, NULL, threadA, NULL);
	printf(" Creation of the thread B\n");
	pthread_create(&thB, NULL, threadB, NULL);
	sleep(1);

	/* The main thread is waiting for A and B to finish */
	printf("The main thread is waiting for A and B to finish\n");
	pthread_join(thA, NULL);
	pthread_join(thB, NULL);

	pthread_exit(NULL);

	exit(EXIT_SUCCESS); /* Unreachable code */
}
