/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * Chapter "Interprocess communication" / Chapitre "Communication interprocessus"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

/**
 * @file test_msg_messages.c
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "message.h"

int main(int argc, char *argv[]) {
	message_t message;
	struct timeval time;
	double begin;
	double end;

	if (argc != 1) {
		fprintf(stderr, "Testing messages\n");
		fprintf(stderr, "Usage: %s \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("\n---------- Beginning of %s ----------\n", argv[0]);

	gettimeofday(&time, NULL);
	begin = time.tv_sec + (time.tv_usec / 1000000.0);

	printf("Filling the message with some A\n");
	msg_fill(&message, 'A');

	printf("Displaying of the message\n");
	msg_display(&message);

	gettimeofday(&time, NULL);
	end = time.tv_sec + (time.tv_usec / 1000000.0);

	printf("\nDuration of the program %s = %.6lf seconds.\n", argv[0], end-begin);

	exit(EXIT_SUCCESS);
}
