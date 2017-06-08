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
 * @file pthread_test.c
 * 
 * A simple program to test threads vs the fork one (see \c fork_test.c).
 *
 * On Mac OS X, compile using gcc -Wall -Wextra pthread_test.c
 * On Linux, compile using gcc -Wall -Wextra pthread_test.c -pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_THREADS 50000
#define OK 0

void *do_little(void *unused)
{
	int i;

	i = 0;
	i = i + 2;

	pthread_exit(NULL);
}

int main(void)
{
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
