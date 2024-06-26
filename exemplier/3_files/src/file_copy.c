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

#include <unistd.h>  /* write and read */
#include <fcntl.h>   /* open, O_CREAT, O_WRONLY */
#include <stdlib.h>  /* exit */
#include <string.h>  /* strerror */
#include <errno.h>   /* errno */

/**
 * @file file_copy.c
 *
 * Copies the text typed on the keyboard to a file named file.out. It reads input 
 * from the keyboard and writes it to the file until there is no more input.
 */

#define SIZE            80
#define STANDARD_ERROR   2
#define STANDARD_OUTPUT  1

int main(void) {
    int fd;
    ssize_t nbcar;
    char buffer[SIZE] = {0};

    fd = open("file.out", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        write(STANDARD_ERROR, strerror(errno), strlen(strerror(errno)));
        write(STANDARD_ERROR, "\n", 1);
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Type your input (Ctrl+D to end):\n", 33);

    while ((nbcar = read(STDIN_FILENO, buffer, SIZE)) > 0) {
        if (write(fd, buffer, (size_t) nbcar) == -1) {
            write(STANDARD_ERROR, strerror(errno), strlen(strerror(errno)));
            write(STANDARD_ERROR, "\n", 1);
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (close(fd) == -1) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
