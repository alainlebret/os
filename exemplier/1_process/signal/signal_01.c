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
 * @file signal_01.c
 *
 * A simple program that uses POSIX signals and handles SIGINT.
 */

#include <stdio.h>  /* printf() */
#include <stdlib.h> /* exit() and execl()*/
#include <unistd.h> /* fork() */
#include <signal.h> /* sigaction() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h> /* wait() */


/** 
 * New handler of the SIGINT signal.
 * @param signal Number of the signal
 */
void handle(int signal) {
	printf("SIGINT signal received!\n");

	exit(EXIT_SUCCESS);
}

int main(void) {
	struct sigaction action;
	action.sa_handler = &handle;

	/* install the new handler of the SIGINT signal */
	sigaction(SIGINT, &action, NULL);

	/* wait for the signal */
	pause();
	printf("This will never be displayed.\n");

	exit(EXIT_SUCCESS);
}
