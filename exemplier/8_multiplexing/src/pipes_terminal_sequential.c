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
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>     /* pipe(), fork(), read(), write(), close(), sleep() */
#include <stdio.h>      /* dprintf(), perror() */
#include <stdlib.h>     /* exit() */
#include <sys/types.h>  /* ssize_t, pid_t */
#include <sys/wait.h>   /* wait() */

/**
 * @file pipes_terminal_sequential.c
 * @brief Running example, version 1: reads two pipes and the terminal one
 * after the other (sequential reading, no multiplexing).
 *
 * Lecture: chapter os-07 « Multiplexage des entrées/sorties »
 * (running example "Version 1 – lecture séquentielle").
 *
 * Two children write 5 messages each into their own pipe (child A every
 * 3 seconds, child B every second). The parent copies each source in turn
 * to the standard output: the pipe of A, the pipe of B, then the terminal.
 *
 * Key points:
 *   - each read() blocks: as long as the user types nothing, neither A nor
 *     B is displayed, even if their data is already waiting in the pipes;
 *   - the display order is imposed by the code, not by the arrivals.
 *
 * Compare with pipes_terminal_select.c and pipes_terminal_poll.c.
 *
 * \code{.bash}
 *   $ ./pipes_terminal_sequential
 *   [A] message 1
 *   [B] message 1
 *   [B] message 2
 *   (nothing more until Enter is pressed)
 * \endcode
 */

/**
 * @brief Handles a fatal error and exits.
 * @param msg Error message displayed before exiting.
 */
void handle_fatal_error_and_exit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Child code: writes 5 messages into the pipe, one every 'delai'
 * seconds, then terminates.
 */
void producteur(int tube[2], const char *nom, int delai) {
    close(tube[0]);          /* child: writer */
    for (int i = 1; i <= 5; i++) {
        sleep(delai);
        dprintf(tube[1], "[%s] message %d\n", nom, i);
    }
    exit(EXIT_SUCCESS);      /* closes tube[1] */
}

/**
 * @brief Reads fd once and copies what was read to the standard output.
 * @return 0 on EOF, -1 on error, otherwise the number of bytes read.
 */
ssize_t copier(int fd) {
    char buf[256];
    ssize_t n = read(fd, buf, sizeof buf);
    if (n > 0) {
        if (write(STDOUT_FILENO, buf, n) == -1) {
            return -1;
        }
    }
    return n;
}

int main(void) {
    int tube_a[2], tube_b[2];
    pid_t pid;

    /* Each pipe is created just before its fork(): B does not inherit
       the writer of A */
    if (pipe(tube_a) == -1) {
        handle_fatal_error_and_exit("pipe");
    }
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {
        producteur(tube_a, "A", 3);   /* does not return */
    }
    close(tube_a[1]);                 /* parent: reader */

    if (pipe(tube_b) == -1) {
        handle_fatal_error_and_exit("pipe");
    }
    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("fork");
    }
    if (pid == 0) {
        producteur(tube_b, "B", 1);
    }
    close(tube_b[1]);

    for (;;) {                        /* each in turn */
        ssize_t a = copier(tube_a[0]);    /* blocks up to 3 s */
        ssize_t b = copier(tube_b[0]);    /* B waits for its turn */
        ssize_t t = copier(STDIN_FILENO); /* blocks until Enter */
        if (a <= 0 && b <= 0 && t <= 0) {
            break;                    /* all three sources are closed */
        }
    }

    close(tube_a[0]);
    close(tube_b[0]);
    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
