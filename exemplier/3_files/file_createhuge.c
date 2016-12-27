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
 * @file file_createhuge.c
 *
 * A simple example that uses the \c lseek primitive to create a huge file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define GIGABYTE 1024*1024*1024
/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	int fd;
	int zero;
	char *filename;
	size_t length;

	/* argc should be 3 */
	if (argc != 3) {
		printf("Usage: %s <filename> <number of gigabytes>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	zero = 0;
	filename = argv[1];
	length = ( size_t )atoi(argv[2]) * GIGABYTE;

	/* Open a new file. */
	fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd == -1) {
		handle_fatal_error("Error opening a file.\n");
	}

	/* Jump to 1 byte of where we want the file to end. */
	lseek(fd, length - 1, SEEK_SET);

	/* Write a single 0 byte.  */
	write(fd, &zero, 1);

	close(fd);

	exit(EXIT_SUCCESS);
}
