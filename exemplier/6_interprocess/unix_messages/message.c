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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @author Bruno Jacob <Bruno.Jacob@lium.univ-lemans.fr>
 * @version	1.1 – 2016-10-01
 */

/**
 * @file message.c
 *
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include "message.h"

/**
 * Displays the type and content of the given message.
 */
void msg_display(message_t *message) {
	printf("\tType: %ld\n", message->type);
	printf("\tContent: %s\n", message->content.buffer);
}

/**
 * Fills the given message vith the specified character.
 */
void msg_fill(message_t *message, char value) {
	int i;

	/* Type of the message */
	message->type = MSG_TYPE_HANDOUT;

	/* Content */
	for (i = 0; i < MESSAGE_SIZE; i++) {
		message->content.buffer[i] = value;
	}

}


