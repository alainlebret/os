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
 * @file fs_block.c
 *
 * A simple example that uses \c statvfs to extract blocks size.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>

int main(void) {
    struct statvfs stat;
    int result;

    result = statvfs("/", &stat);
    if (result == -1) {
        exit(EXIT_FAILURE);
    }
    printf("FS block size: %d bytes \n", (int) (stat.f_bsize));

    exit(EXIT_SUCCESS);
}
