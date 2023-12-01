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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

#define FOREVER for (;;)

/**
 * @file lock_file.c
 *
 * Example using file locking.
 * \code{.bash}
 *   $ touch test_lock
 *   $ lock_file & lock_file
 *   19357 has locked the file
 *   19358 found the file already locked, try again...
 *   19358 found the file already locked, try again...
 *   19358 found the file already locked, try again...
 *   19357 has unlocked the file
 *   19358 has locked the file
 *   19358 has unlocked the file
 * \endcode
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int open_lockfile(const char *name)
{
    int fd;

    fd = open(name, O_RDWR); /* must exist */
    if (fd == -1) {
        handle_fatal_error("Error [open()]: ");
    }

    return fd;
}

int main(void)
{
    int fd;
    pid_t pid;

    pid = getpid();

    fd = open_lockfile("test_lock");

    FOREVER {
        if (lockf(fd, F_TLOCK, 0) == 0) {
            printf("%d has locked the file\n", pid);
            sleep(5);

            if (lockf(fd, F_ULOCK, 0) == 0) {
                printf("%d has unlocked the file\n", pid);
            }

            exit(EXIT_SUCCESS);

        } else {
            printf("%d found the file already locked, try again...\n", pid);
            sleep(2);
        }
    }

    /* Unreachable: use <Ctrl-C> to exit */
}
