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

#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* exit() */
#include <fcntl.h>      /* open() opening flags and file modes */
#include <unistd.h>     /* close() */
#include <sys/stat.h>
#include <sys/types.h>

#define GIGABYTE (1024LL * 1024 * 1024)

/**
 * @file file_createhuge.c
 *
 * A simple example that uses the lseek() primitive to create a huge file.
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
    int zero;
    char *filename;
    size_t length;

    /* argc should be 3 */
    if (argc != 3) {
        printf("Usage: %s <filename> <number of gigabytes>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    zero = 0;
    filename = argv[1];
    int gigabytes = atoi(argv[2]);
    if (gigabytes <= 0) {
        fprintf(stderr, "Number of gigabytes must be positive.\n");
        exit(EXIT_FAILURE);
    }

    length = (off_t)gigabytes * GIGABYTE;

    /* Open a new file. */
    fd = open(filename, O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        handle_fatal_error_and_exit("Error [open()]: ");
    }

    /* Jump to where we want the file to end. */
    if (lseek(fd, length - 1, SEEK_SET) == -1) {
        handle_fatal_error_and_exit("Error [lseek()]: ");
    }

    /* Write a single 0 byte. */
    if (write(fd, &zero, 1) != 1) {
        handle_fatal_error_and_exit("Error [write()]: ");
    }

    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    printf("Successfully created a %d gigabyte file '%s'.\n", gigabytes, filename);

    return EXIT_SUCCESS;
}
