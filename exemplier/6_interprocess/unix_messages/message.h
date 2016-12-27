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
 * @file message.h
 *
 *
 */
#ifndef HANDOUT_MESSAGE_H
#define HANDOUT_MESSAGE_H

#define MESSAGE_SIZE 1024
#define MSG_TYPE_HANDOUT 22

/** Structure of a message */
typedef struct content {
	char buffer[MESSAGE_SIZE];
} content_t;

typedef struct message {
	long type;
	content_t content;
} message_t;

/**
 * Displays the type and content of the given message.
 */
void msg_display(message_t *message);

/**
 * Fills the given message vith the specified character.
 */
void msg_fill(message_t *message, char val);

#endif
