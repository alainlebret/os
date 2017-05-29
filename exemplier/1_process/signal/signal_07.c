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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file signal_07.c
 *
 * A simple program that uses POSIX signals and handles the SIGUSR1 signal.
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait() */
#include <signal.h>    /* sigaction */

unsigned char nb_calls = 7;

/**
 * Handles the signal SIGUSR1 by decrementing \em nbCalls.
 * @param signal Number of the received signal.
 */
void handleSevenLifes(int signal)
{
	nb_calls--;
	printf("Still have %d lifes.\n", (int) nb_calls);
}

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. Decrements the number of lifes before sending
 * the signal SIGUSR1 to his child.
 */
void manage_parent(pid_t child)
{
	printf("Parent process (PID %d)\n", getpid());

	while (nb_calls-- != 0) {
		sleep(2);
		printf("Parent: sending SIGUSR1 to the child %d...\n", child);
		kill(child, SIGUSR1);
	}

	exit(EXIT_SUCCESS);
}

/**
 * Manages the child process. Configures the handler to react to the signal
 * SIGUSR1. Child exits when \em nbCalls reaches 0.
 */
void manage_child()
{
	struct sigaction managingLifes;

	printf("Child process (PID %d)\n", getpid());

	managingLifes.sa_handler = &handleSevenLifes;
	sigaction(SIGUSR1, &managingLifes, NULL);

	while (nb_calls != 0);

	exit(EXIT_SUCCESS);
}

int main(void)
{
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		handle_fatal_error("Error using fork().\n");
	}
	if (pid > 0) {
		manage_parent(pid);
	} else {
		manage_child();
	}

	exit(EXIT_SUCCESS);
}
