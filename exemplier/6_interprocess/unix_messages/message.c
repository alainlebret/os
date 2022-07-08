/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @author Bruno Jacob <Bruno.Jacob@lium.univ-lemans.fr>
 * @version	1.1 – 2016-10-01
 */

/**
 * @file message.c
 */

 /**
 * Displays the type and content of the given message.
 */
void msg_display(message_t *message)
{
    printf("\tType: %ld\n", message->type);
    printf("\tContent: %s\n", message->content.buffer);
}

/**
 * Fills the given message vith the specified character.
 */
void msg_fill(message_t *message, char value)
{
    int i;

    /* Type of the message */
    message->type = MSG_TYPE_HANDOUT;

    /* Content */
    for (i = 0; i < MESSAGE_SIZE; i++) {
        message->content.buffer[i] = value;
    }

}
