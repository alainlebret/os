/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  /* strcmp() */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "message.h"

/**
 * @file unix_msg_recv.c
 *
 * Consumer program using a System V IPC message mechanism.
 * Run unix_msg_send first (it creates the queue). The queue persists after
 * the programs end: observe it with "ipcs -q", remove it with "ipcrm -q <id>",
 * or run "./unix_msg_recv -r", which removes it after reading (msgctl()).
 */

#define MSGQ_KEY 1234
#define MSGQ_PERM 0600
/* Fixed key for the demonstration (see ftok() in the course); permissions
 * 0600: only the owner can use the queue. */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Returns the message queue ID for the given key.
 */
int get_msgq_id(key_t key) {
    int msgq_id;
    int msg_flag;

    msg_flag = MSGQ_PERM;
    msgq_id = msgget(key, msg_flag);

    if (msgq_id < 0) {
        handle_fatal_error("Error using msgget()!");
    }

    return msgq_id;
}

int main(int argc, char *argv[]) {
    int msgq_id;
    message_t message;
    int remove_queue = (argc == 2 && strcmp(argv[1], "-r") == 0);

    msgq_id = get_msgq_id(MSGQ_KEY);

    /* Receive an answer of message type MSG_TYPE_HANDOUT */
    if (msgrcv(msgq_id, &message, MESSAGE_SIZE, MSG_TYPE_HANDOUT, 0) < 0) {
        handle_fatal_error("Error using msgrcv()!");
    }

    msg_display(&message);

    if (remove_queue) {
        if (msgctl(msgq_id, IPC_RMID, NULL) == -1) {
            handle_fatal_error("Error using msgctl(IPC_RMID)");
        }
        printf("Queue removed.\n");
    }

    return EXIT_SUCCESS;
}
