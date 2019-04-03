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
 * @file barrier.c
 *
 * A simple program to show pthread_barrier synchronization.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_COUNT 4

pthread_barrier_t barrier;

void* threadFn(void *id_ptr) {
  int thread_id = *(int*)id_ptr;
  int wait_sec = 1 + rand() % 5;
  printf("thread %d: wait for %d seconds.\n", thread_id, wait_sec);
  sleep(wait_sec);
  printf("thread %d: Ready...\n", thread_id);

  pthread_barrier_wait(&barrier);

  printf("thread %d: passed the barrier!\n", thread_id);
  return NULL;
}

int main(void) 
{
	int i;
	pthread_t ids[THREAD_COUNT];
	int short_ids[THREAD_COUNT];

	srand(time(NULL));
	pthread_barrier_init(&barrier, NULL, THREAD_COUNT + 1);

	for (i = 0; i < THREAD_COUNT; i++) {
		short_ids[i] = i;
		pthread_create(&ids[i], NULL, threadFn, &short_ids[i]);
	}

	printf("The main thread is ready.\n");

	pthread_barrier_wait(&barrier);

	printf("The main thread passed the barrier!\n");

	for (i = 0; i < THREAD_COUNT; i++) {
		pthread_join(ids[i], NULL);
	}

	pthread_barrier_destroy(&barrier);

	exit(EXIT_SUCCESS);
}
