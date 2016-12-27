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
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	printf(msg);
	exit(EXIT_FAILURE);
}

int main (int argc, char* argv[]) {
	char* filename; /* The path at which to create the new file.  */
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
		handle_fatal_error("Error opening a file.\n");
	}

	close(fd);

	exit(EXIT_SUCCESS);
}