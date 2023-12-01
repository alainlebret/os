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
#include <sys/statvfs.h>

/**
 * @file fs_block.c
 *
 * A simple example that uses \c statvfs to extract blocks size.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

int main(int argc, char *argv[])
{
    struct statvfs fs_stats;
    int result;

    if (argc != 2) {
        printf("Usage: %s <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    result = statvfs(argv[1], &fs_stats);
    if (result == -1) {
        perror("Error [statvfs]: ");
        exit(EXIT_FAILURE);
    }
    
    printf("FS block size: %lu bytes\n", fs_stats.f_bsize);

    return EXIT_SUCCESS;
}
