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
#include <unistd.h>    /* lseek(), write(), close() */
#include <fcntl.h>     /* open() opening flags and file modes */
#include <sys/mman.h>  /* mmap() */
#include <sys/stat.h>  /* stat() */
#include <sys/types.h>
#include <string.h>    /* memcpy() */

/**
 * @file mmap2.c
 *
 * This program replicates the functionality of the cp command by using memory
 * mapping. It maps the contents of a source file into memory, then maps a 
 * destination file into memory, and performs a memory copy from the source to
 * the destination.
 */

/**
 * Get the size of the file by its filename using stat().
 */
long get_file_size(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == -1) {
        perror("Error using stat()");
        exit(EXIT_FAILURE);
    }

    return (long) st.st_size;
}

int main(int argc, char *argv[]) {
    int fdin;
    int fdout;
    char *src;
    char *dst;
    long file_size;

    if (argc != 3) {
        printf("Usage: mmap2 <src> <dest>\n");
        exit(EXIT_FAILURE);
    }

    file_size = get_file_size(argv[1]);

    if (file_size == 0) {  /* an empty file cannot be projected (EINVAL) */
        fprintf(stderr, "%s: empty file\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    fdin = open(argv[1], O_RDONLY);
    if (fdin == -1) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }
    fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fdout == -1) {
        perror("Error opening destination file");
        exit(EXIT_FAILURE);
    }

    /* Give the destination file its size: go to the last byte and write
     * one char there (ftruncate(fdout, file_size) would do the same) */
    if (lseek(fdout, file_size - 1, SEEK_SET) == -1 ||
        write(fdout, "", 1) == -1) {
        perror("Error sizing destination file");
        exit(EXIT_FAILURE);
    }

    /* Project the input file in memory */
    src = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fdin, 0);
    if (src == MAP_FAILED) {
        perror("Error using mmap() on source");
        exit(EXIT_FAILURE);
    }

    /* Same for the output one */
    dst = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
    if (dst == MAP_FAILED) {
        perror("Error using mmap() on destination");
        exit(EXIT_FAILURE);
    }

    /* Performs a memory copy from src to dst */
    memcpy(dst, src, file_size);

    munmap(src, file_size);
    munmap(dst, file_size);   /* the copy is carried over to the file */
    close(fdin);
    close(fdout);

    return EXIT_SUCCESS;
}
