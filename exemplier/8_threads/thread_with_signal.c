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
 * @file thread_with_signal.c
 *
 * A simple program using threads and signal.
 *
 * On Mac OS X, compile with gcc -Wall -Wextra -ansi -pedantic thread_with_signal.c
 * On Linux, compile with gcc -Wall -Wextra -ansi -pedantic thread_with_signal.c -lpthread
 */

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>

#define EVER ;;

typedef struct data
{
	char name[10];
	int age;
} data_t;

void handle_signal(int sig)
{
	write(1, "Caught signal SIGINT\n", 21);
	pthread_exit(NULL);
}

void func(data_t *p)
{
	for (EVER) {
		fprintf(stderr, "This is from thread function\n");
		strcpy(p->name, "Mr. Linux");
		p->age = 30;
		sleep(2);
	}
}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;
	data_t d;
	data_t *ptr = &d;
	struct sigaction action;

	action.sa_handler = &handle_signal;
	sigaction(SIGINT, &action, NULL);

	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, (void*)func, ptr);
	sleep(10);
	pthread_kill(tid, SIGINT);

	pthread_join(tid, NULL);
	fprintf(stderr, "Name: %s\n",ptr->name);
	fprintf(stderr, "Age: %d\n",ptr->age);
}