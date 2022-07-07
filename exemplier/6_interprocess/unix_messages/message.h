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
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
