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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* write(), close() */
#include <fcntl.h>     /* open() opening flags and file modes */
#include <sys/mman.h>  /* mmap() */
#include <sys/stat.h>  /* stat() */
#include <sys/types.h>
#include <ctype.h>     /* isalpha(), isspace() */
#include <assert.h>

/**
 * @file mmap1.c
 *
 * This program demonstrates memory mapping of a file using mmap(). It maps the
 * contents of a specified file into memory, writes the content to the terminal,
 * processes the data, and then unmaps the memory.
 */

#define TERMINAL STDOUT_FILENO

/**
 * Get the size of an open file using fstat(): open() first, then fstat() on
 * the descriptor, so that the size is that of the file actually opened.
 */
long get_file_size(int fd) {
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error using fstat()");
        exit(EXIT_FAILURE);
    }
    return st.st_size;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd;
    long file_size;
    char *projection;

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    file_size = get_file_size(fd);

    projection = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
    if (projection == MAP_FAILED) {
        perror("Error using mmap()");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (write(TERMINAL, projection, file_size) == -1) {
        perror("Error using write()");
        munmap(projection, file_size);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* The projection is read like an array: here, only the characters that
       are neither letters nor spaces (digits, punctuation) are displayed */
    for (long i = 0; i < file_size; i++) {
        if (i % 80 == 79) {
            putchar('\n');
        } else if (!isalpha((unsigned char) projection[i]) &&
                   !isspace((unsigned char) projection[i])) {
            putchar(projection[i]);
        }
    }

    munmap(projection, file_size);
    close(fd);

    return EXIT_SUCCESS;
}
