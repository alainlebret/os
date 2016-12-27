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
 * @file file_hexdump.c
 *
 * An example of using \c the read primitive to display the hexadecimal dump of
 * a file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 20

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	printf(msg);
	exit(EXIT_FAILURE);
}

int main (int argc, char* argv[]) {
	int fd;
	char *filename;
	unsigned char buffer[BUFFER_SIZE];
	size_t offset;
	ssize_t bytes_read;
	int i;

	/* argc should be 2 */
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Open the file for reading. */
	filename = argv[1];
	fd = open(filename, O_RDONLY);
	if (fd == -1) {
		handle_fatal_error("Error opening a file.\n");
	}

	offset = 0;

	do {
		/* Read the next line of 20 bytes.  */
		bytes_read = read(fd, buffer, sizeof(buffer));

		/* Display the offset in the file followed by the bytes themselves.  */
		printf("0x%06x : ", offset);

		for (i = 0; i < bytes_read; ++i)
			printf("%02x ", buffer[i]);
		printf("\n");

		/* Keep position in the file.  */
		offset += bytes_read;
	} while (bytes_read == sizeof (buffer));

	close (fd);

	exit(EXIT_SUCCESS);
}
