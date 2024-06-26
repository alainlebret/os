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
#include <stdlib.h>
#include <sys/statvfs.h> /* statvfs() */

/**
 * @file fs_block.c
 *
 * Demonstrates how to use statvfs() to extract the block size of a 
 * filesystem. The block size is a key metric for understanding filesystem
 * allocation and performance.
 */

int main(int argc, char *argv[]) {
    struct statvfs fs_stats;
    int result;

    if (argc != 2) {
        printf("Usage: %s <path to any directory or file on the target filesystem>\n", argv[0]);
        printf("This program returns the block size of the filesystem containing the given path.\n");
        exit(EXIT_FAILURE);
    }

    result = statvfs(argv[1], &fs_stats);
    if (result == -1) {
        perror("Error retrieving filesystem statistics");
        exit(EXIT_FAILURE);
    }

    printf("Filesystem block size: %lu bytes\n", fs_stats.f_bsize);
    printf("Total blocks: %lu\n", fs_stats.f_blocks);
    printf("Free blocks: %lu\n", fs_stats.f_bfree);
    printf("Available blocks (non-superuser): %lu\n", fs_stats.f_bavail);

    return EXIT_SUCCESS;
}
