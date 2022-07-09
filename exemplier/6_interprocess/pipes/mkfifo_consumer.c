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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* pour fork() */
#include <sys/types.h> /* pid_t and mkfifo() */
#include <sys/stat.h>  /* mkfifo() */
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 100

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file mkfifo_consumer.c
 *
 * A consumer program that uses a named pipe (mkfifo) to receive messages from
 * a producer.
 */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Open the pipe with the specified name in a read only mode.
 * @param name Name of the pipe.
 * @return The pipe descriptor.
 */
int open_pipe(const char *name)
{
    int pd;

    pd = open(name, O_RDONLY);
    if (pd == -1) {
        handle_fatal_error("Error [open()]: ");
    }
    return pd;
}

int main(void)
{
    char buffer[BUFFER_SIZE];
    int pd;
    ssize_t message_length;

    pd = open_pipe("./testfifo");

    strcpy(buffer, "");
    message_length = read(pd, buffer, BUFFER_SIZE);

    if (message_length > 0) {
        printf("\nRead message from the pipe...\n");
        printf("%s\n", buffer);
    } else {
        printf("Named pipe is empty.\n");
    }

    close(pd);
    unlink("./testfifo");

    exit(EXIT_SUCCESS);
}

