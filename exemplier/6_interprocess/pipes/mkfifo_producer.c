/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Files, pipes and fifo" / "Fichiers et entrées-sorties"
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
 * @file mkfifo_producer.c
 *
 * A producer program that uses a named pipe (FIFO) to send messages to a
 * consumer.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour exit() et execl()*/
#include <unistd.h> /* pour fork() */
#include <sys/types.h> /* pour pid_t et mkfifo() */
#include <sys/stat.h> /* pour mkfifo() */
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 100

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Creates a named pipe.
 * @param name Name of the pipe.
 * @param mode Permissions access.
 * @return The pipe identifier.
 */
int create_pipe(const char *name, mode_t mode) {
	int pipe;

	pipe = mkfifo(name, mode);

	return pipe;
}

/**
 * Open the pipe with the specified name in a write only mode.
 * @param name Name of the pipe.
 * @return The pipe descriptor.
 */
int open_pipe(const char *name) {
	int pd;

	pd = open(name, O_WRONLY);

	return pd;
}

int main(void) {
	char buffer[BUFFER_SIZE];
	int pd;
	int pipe;

	/*
	 * Create a new pipe named "testfifo" with read/write permissions for owner,
	 * and with read permissions for group and others.
	 */
	pipe = create_pipe("./testfifo", S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	if (pipe == -1) {
		handle_fatal_error("\nError when trying to create the named pipe.\n");
	}

	pd = open_pipe("./testfifo");
	if (pd == -1) {
		handle_fatal_error("\nError when trying to open the named pipe.\n");
	}


	strcpy(buffer,
			 "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium...");
	write(pd, buffer, BUFFER_SIZE);

	close(pd);
	unlink("./testfifo");

	exit(EXIT_SUCCESS);
}
