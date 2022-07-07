/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Files, pipes and fifo" / "Fichiers et entrées-sorties"
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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file file_hexdump.c
 *
 * An example of using \c the read primitive to display the hexadecimal dump of
 * a file.
 */

#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* exit() */
#include <fcntl.h>      /* open() opening flags and file modes */
#include <unistd.h>     /* read() and close() */
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 20

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int fd; /* file descriptor */
    char *filename;
    unsigned char buffer[BUFFER_SIZE];
    size_t offset;
    ssize_t bytes_read;
    int i;

    /* argc should be 2 */
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Open the file for reading. */
    filename = argv[1];
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        handle_fatal_error_and_exit("Error opening a file.\n");
    }

    offset = 0;

    do {
        /* Read the next line of 20 bytes.  */
        bytes_read = read(fd, buffer, sizeof(buffer));

        /* Display the offset in the file followed by the bytes themselves.  */
        printf("0x%06x : ", (unsigned int) offset);

        for (i = 0; i < bytes_read; ++i) {
            printf("%02x ", buffer[i]);
        }
        printf("\n");

        /* Keep position in the file.  */
        offset += bytes_read;
    } while (bytes_read == sizeof(buffer));

    close(fd);

    exit(EXIT_SUCCESS);
}
