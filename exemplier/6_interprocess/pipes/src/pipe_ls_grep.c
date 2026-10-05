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
#include <stdio.h>     /* perror() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* pipe(), fork(), dup2(), close(), execlp() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid() */

/**
 * @file pipe_ls_grep.c
 * @brief Reproduces the shell pipeline "ls | grep txt".
 *
 * Course "Operating Systems", chapter « Tubes », slides
 * « Pipeline (1/2) : enfant 1, ls » and « Pipeline (2/2) : enfant 2, grep,
 * et parent ».
 *
 * The parent creates the pipe and two children:
 * - child 1 redirects its stdout to the write end, then executes ls;
 * - child 2 redirects its stdin to the read end, then executes grep txt.
 * The parent closes both ends: if it kept fd[1], grep would never read 0
 * (EOF) and would wait forever.
 *
 * \code{.bash}
 *   $ touch a.txt b.txt c.dat
 *   $ ./pipe_ls_grep
 *   a.txt
 *   b.txt
 * \endcode
 */

int main(void) {
    int fd[2];
    int status;

    if (pipe(fd) == -1) { perror("pipe"); exit(EXIT_FAILURE); }

    pid_t pid1 = fork();
    if (pid1 == -1) { perror("fork"); exit(EXIT_FAILURE); }
    if (pid1 == 0) {                /* child 1: ls */
        close(fd[0]);               /* read end unused */
        dup2(fd[1], STDOUT_FILENO); /* stdout -> pipe */
        close(fd[1]);
        execlp("ls", "ls", (char *) NULL);
        perror("execlp");           /* exec failed */
        _exit(127);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) { perror("fork"); exit(EXIT_FAILURE); }
    if (pid2 == 0) {                /* child 2: grep */
        close(fd[1]);               /* write end unused */
        dup2(fd[0], STDIN_FILENO);  /* stdin <- pipe */
        close(fd[0]);
        execlp("grep", "grep", "txt", (char *) NULL);
        perror("execlp");
        _exit(127);
    }

    close(fd[0]);                   /* parent: does not use */
    close(fd[1]);                   /* the pipe */
    waitpid(pid1, NULL, 0);
    if (waitpid(pid2, &status, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    /* Like the shell, return the status of the last command */
    return WIFEXITED(status) ? WEXITSTATUS(status) : EXIT_FAILURE;
}
