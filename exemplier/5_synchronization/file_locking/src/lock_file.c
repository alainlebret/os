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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

/**
 * @file lock_file.c
 *
 * This program demonstrates file locking by attempting to lock the file
 * /tmp/test_lock. If the file is already locked by another process, it waits
 * one second and retries until it can acquire the lock. Once locked, it holds
 * the lock for 3 seconds (simulated work) before unlocking and exiting.
 * Same program as the slides « lockf() – exemple » and « lockf() – exécution ».
 *
 * \code{.bash}
 *   $ touch /tmp/test_lock
 *   $ ./lock_file & ./lock_file
 *   19357 : verrouillé
 *   19358 : déjà verrouillé
 *   19358 : déjà verrouillé
 *   19358 : déjà verrouillé
 *   19357 : déverrouillé
 *   19358 : verrouillé
 *   19358 : déverrouillé
 * \endcode
 */

/**
 * Handles a fatal error by displaying a message, then exits.
 */
void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int open_lockfile(const char *name) {
    int fd = open(name, O_RDWR);  /* File must exist */
    if (fd == -1) {
        handle_fatal_error("Error opening file");
    }
    return fd;
}

int main(void) {
    int fd = open_lockfile("/tmp/test_lock");
    pid_t pid = getpid();

    while (1) {
        if (lockf(fd, F_TLOCK, 0) == -1) {
            if (errno == EACCES || errno == EAGAIN) {
                printf("%ld : déjà verrouillé\n", (long) pid);
                sleep(1);
            } else {
                handle_fatal_error("Error locking file");
            }
        } else {
            printf("%ld : verrouillé\n", (long) pid);
            sleep(3);   /* simulated work, lock held */

            if (lockf(fd, F_ULOCK, 0) == -1) {
                handle_fatal_error("Error unlocking file");
            }
            printf("%ld : déverrouillé\n", (long) pid);
            break;  /* Exit after unlocking the file */
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
