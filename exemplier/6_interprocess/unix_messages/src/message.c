/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <unistd.h>
#include <sys/msg.h>

#include "message.h"

/**
 * @file message.c
 *
 * @author Alain Lebret
 * @author Bruno Jacob <Bruno.Jacob@lium.univ-lemans.fr>
 * @version	1.1 – 2016-10-01
 */

/**
 * Displays the type and content of the given message.
 * @param message Pointer to the message to be displayed.
 */
void msg_display(message_t *message)
{
    if (message == NULL) {
        fprintf(stderr, "Error: NULL pointer provided to msg_display().\n");
        return;
    }

    printf("\tType: %ld\n", message->type);
    printf("\tContent: %s\n", message->content.buffer);
}

/**
 * Fills the given message with the specified character.
 * @param message Pointer to the message to be filled.
 * @param value Character used to fill the message buffer.
 */
void msg_fill(message_t *message, char value)
{
    int i;

    if (message == NULL) {
        fprintf(stderr, "Error: NULL pointer provided to msg_fill.\n");
        return;
    }

    /* Type of the message */
    message->type = MSG_TYPE_HANDOUT;

    /* Content */
    for (i = 0; i < MESSAGE_SIZE; i++) {
        message->content.buffer[i] = value;
    }
}
