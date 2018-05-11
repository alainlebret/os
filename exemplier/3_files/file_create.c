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
 * @file file_create.c
 *
 * A simple program that shows using the \c open primitive to create a new file.
 */

#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* exit() */
#include <fcntl.h>      /* open() opening flags and file modes */
#include <unistd.h>     /* close() */
#include <sys/stat.h>
#include <sys/types.h>

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	char *filename; /* The path at which to create the new file.  */
	mode_t mode; /* The permissions for the new file.  */

	/* argc should be 2 */
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	/* Create the file.  */
	int fd = open(filename, O_WRONLY | O_EXCL | O_CREAT, mode);
	if (fd == -1) {
		handle_fatal_error_and_exit("Error opening a file.\n");
	}

	close(fd);

	exit(EXIT_SUCCESS);
}