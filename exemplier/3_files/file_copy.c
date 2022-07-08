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

#include <unistd.h> /* write and read */
#include <fcntl.h>  /* open, O_CREAT, O_WRONLY */
#include <stdlib.h> /* exit */

#define SIZE            80
#define STANDARD_ERROR   2
#define STANDARD_OUTPUT  1
#define KEYBOARD         0

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file file_copy.c
 *
 * Copy typed keys from keyboard to a file.
 */

int main(void)
{
    int fd;
    ssize_t nbcar;
    char buffer[SIZE];

    fd = open("file.out", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        write(STANDARD_ERROR, "Error opening a file\n", 25);
        exit(EXIT_FAILURE);
    }

    write(STANDARD_OUTPUT, "File opened with success\n", 30);

    while ((nbcar = read(KEYBOARD, buffer, SIZE)) > 0) {
        if (write(fd, buffer, (size_t) nbcar) == -1) {
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
