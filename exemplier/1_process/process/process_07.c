/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Processes & signals" / "Processus et signaux"
 *
 * Copyright (C) 1995-2018 Alain Lebret (alain.lebret@ensicaen.fr)
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
 * @version	1.1
 * @date 2017-12-31
 */

/**
 * @file process_07.c
 *
 * A simple program about a process and its group.
 */
#include <stdint.h>    /* C11 int types */
#include <inttypes.h>  /* C11 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* getpid() and getpgrp() */
#include <sys/types.h> /* pid_t */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Manages the parent process. Parent is waiting for his child.
 */
void manage_parent()
{
	pid_t child;
	int32_t status;

	printf("Parent process: %" PRId32 "\n", getpid());
	printf("Parent group: %" PRId32 "\n", getpgrp());
	child = wait(&status);
	if (WIFEXITED(status)) {
		printf("%d : child %" PRId32 " has finished is work (code: %" PRId32 ")\n", 
			getpid(), child, WEXITSTATUS(status));
	}
}

/**
 * Manages the child process. Child is calling the exec function to execute the
 * \em gnuplot program.
 */
void manage_child()
{
	printf("Child process: %" PRId32 "\n", getpid());
	printf("Child group: %" PRId32 "\n", getpgrp());
}

int main(void)
{
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		handle_fatal_error("Error using fork().\n");
	}
	if (pid > 0) {
		manage_parent();
	} else {
		manage_child();
	}

	exit(EXIT_SUCCESS);
}
