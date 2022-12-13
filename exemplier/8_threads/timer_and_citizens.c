/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Inspired from Lloyd Rochester.
 * See: https://lloydrochester.com
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* timer_begin_new_day(void *data);
void* citizen_spends_his_day(void *data);

pthread_mutex_t day_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t citizen_can_wake_up_cond = PTHREAD_COND_INITIALIZER;
int thread_ids[6] = { 0, 1, 2, 3, 4, 5 };

int main(int argc, char *argv[])
{
	int i;
	pthread_t threads[6];

	for (i = 1; i < 6; i++) {
		pthread_create(&threads[i], NULL, citizen_spends_his_day, &thread_ids[i]);
	}
	
	/* 
	 * the citizen threads have already started and are waiting
	 * on the condition.
	 */
	sleep(1);
	pthread_create(&threads[0], NULL, timer_begin_new_day, &thread_ids[0]);

	for (i = 0; i < 6; i++) {
		pthread_join(threads[i], NULL);
		printf("thread %i joined\n",i);
	}

	return 0;
}

void *timer_begin_new_day(void *data)
{
	int *id = (int *) data;

	pthread_mutex_lock(&day_mutex);
	printf("Timer %d wakes up citizens by broadcasting\n",*id);
	pthread_cond_broadcast(&citizen_can_wake_up_cond);
	pthread_mutex_unlock(&day_mutex);

	return NULL;
}

void *citizen_spends_his_day(void *data)
{
	int *id = (int *) data;

	pthread_mutex_lock(&day_mutex);
	printf("The citizen %d is sleeping\n",*id);
	pthread_cond_wait(&citizen_can_wake_up_cond, &day_mutex);
	printf("The citizen %d woke up\n",*id);
	pthread_mutex_unlock(&day_mutex);

	printf("The citizen %d spends his day...\n",*id);
	/* Doing a lot of things */
	
	return NULL;
}

