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
#include <stdlib.h>    /* exit() */
#include <fcntl.h>     /* open() opening flags and file modes */
#include <unistd.h>    /* read() and close() */
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 20

/**
 * @file file_hexdump.c
 *
 * An example of using \c the read primitive to display the hexadecimal dump of
 * a file.
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int fd;
    unsigned char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    size_t offset = 0;
    int i;

    if (argc != 2) {
        printf("Usage: %s <filename> - Display the hexadecimal dump of a file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        handle_fatal_error_and_exit("Error opening file: ");
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        printf("0x%06zx : ", offset);
        for (i = 0; i < bytes_read; ++i) {
            printf("%02x ", buffer[i]);
        }
        printf(" | ");
        for (i = 0; i < bytes_read; ++i) {
            printf("%c", (buffer[i] >= 32 && buffer[i] <= 126) ? buffer[i] : '.');
        }
        printf("\n");
        offset += bytes_read;
    }

    if (bytes_read == -1) {
        close(fd);
        handle_fatal_error_and_exit("Error reading file: ");
    }

    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
