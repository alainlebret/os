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
 * @file unix_msg_send.c
 *
 * Producer program using a System V IPC message mechanism.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Returns the message queue ID for the given key
 */
int get_msgq_id(key_t key) {
	int msgq_id;
	int msg_flag;

	msg_flag= IPC_CREAT | 0666;

	msgq_id = msgget(key, msg_flag);
	if (msgq_id < 0) {
		handle_fatal_error("Error using msgget()\n");
	}

	return msgq_id;
}

int main(void) {
	int msgq_id;
	message_t message;
	size_t length;

	msgq_id = get_msgq_id(1234);

	/* Message type */
	message.type = MSG_TYPE_HANDOUT;

	printf("Enter a message to add to message queue: ");
	scanf("%[^\n]", message.content.buffer);
	getchar();

	length = strlen(message.content.buffer) + 1;

	if (msgsnd(msgq_id, &message, length, IPC_NOWAIT) < 0) {
		printf("%d, %ld, %s, %lu\n", msgq_id, message.type, message.content.buffer,
				 length);
		handle_fatal_error("Error using msgsnd()\n");
	} else {
		printf("Message sent\n");
	}

	exit(EXIT_SUCCESS);
}
