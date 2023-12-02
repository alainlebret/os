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
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t and mkfifo() */
#include <sys/stat.h>  /* mkfifo() */
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

/**
 * @file mkfifo_producer.c
 *
 * A producer program that uses a named pipe (FIFO) to send messages to a
 * consumer.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2011-12-01
 */

#define BUFFER_SIZE 100

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * Creates a named pipe.
 * @param name Name of the pipe.
 * @param mode Permissions access.
 * @return The pipe identifier.
 */
int create_pipe(const char *name, mode_t mode)
{
    if (mkfifo(name, mode) == -1) {
        handle_fatal_error("Error [mkfifo()]: ");
    }
    return 0; /* If mkfifo is successful */
}

/**
 * Open the pipe with the specified name in a write only mode.
 * @param name Name of the pipe.
 * @return The pipe descriptor.
 */
int open_pipe(const char *name)
{
    int pd = open(name, O_WRONLY);
    if (pd == -1) {
        handle_fatal_error("Error [open()]: ");
    }
    return pd;
}

int main(void)
{
    char buffer[BUFFER_SIZE] = "Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium...";
    ssize_t bytes_written;

   /*
    * Create a new pipe named "testfifo" with read/write permissions for owner,
    * and with read permissions for group and others.
    */
    create_pipe("testfifo", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    int pd = open_pipe("testfifo");

    bytes_written = write(pd, buffer, strlen(buffer));
    if (bytes_written == -1) {
        handle_fatal_error("Error writing to pipe");
    }

    close(pd);
    /* Consider if unlink("testfifo"); is appropriate here */
    /* unlink("testfifo"); */

    return EXIT_SUCCESS;
}
