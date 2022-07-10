/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork(), close() */
#include <fcntl.h>     /* open() opening flags and file modes */
#include <sys/mman.h>  /* mmap() */
#include <sys/stat.h>  /* stat() */
#include <sys/types.h>
#include <ctype.h>     /* isalpha(), isspace() */
#include <assert.h>

#define TERMINAL 1

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file mmap1.c
 *
 * A memmory mapping example using the \c mmap primitive.
 */

long get_file_size(const char *filename)
{
    struct stat st;

    stat(filename, &st);
    printf("%ld\n", st.st_size);

    return (long int) st.st_size;
}

int main(int argc, char **argv)
{
    int fd;
    long i;
    long file_size;
    char *projection;

    file_size = get_file_size(argv[1]);

    fd = open(argv[1], O_RDONLY, 0);

    /* Project the file content to memory */
    projection = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
    assert(projection != MAP_FAILED);

    /* Write the content to the terminal */
    if (write(TERMINAL, projection, file_size) == -1) {
        perror("Error using write(): ");
    }

    /* Do some stuff with data */
    for (i = 0; i < file_size; i++) {
        char c;

        c = projection[i];
        printf("%c", c);
        if (!isalpha(c) && !isspace(c)) {
            putchar(c);
        }
        if (i % 80 == 79) {
            putchar('\n');
        }
    }

    /* Unmapping memory */
    munmap(projection, file_size);

    close(fd);

    exit(EXIT_SUCCESS);
}

