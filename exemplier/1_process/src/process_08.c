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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() */
#include <unistd.h>    /* fork(), execv() and _exit() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid() */

/**
 * @file process_08.c
 *
 * A simple program about a process that executes Gtk windows through its children.
 */

/* Built by the Makefile in bin/; run this program from 1_process/ */
static char *path = "./bin/moving_window";

/*
 * This block will be executed by the first child
 */
void manage_child1(void) {
    /* Arguments for the GTK application */
    char *args[] = {"moving_window", "100", "100", "#5bccc9", NULL};

    /* Execute the GTK application: path is given, no PATH search, so execv() */
    execv(path, args);

    /* If execv() fails */
    perror("execv failed for child 1");
    _exit(127);
}

/*
 * This block will be executed by the second child
 */
void manage_child2(void) {
    /* Arguments for the GTK application */
    char *args[] = {"moving_window", "350", "100", "#bca850", NULL};

    /* Execute the GTK application */
    execv(path, args);

    /* If execv() fails */
    perror("execv failed for child 2");
    _exit(127);
}

int main(void) {
    pid_t pid1, pid2;

    /* First fork to create the first child */
    pid1 = fork();

    if (pid1 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        manage_child1(); /* never returns: exec or _exit() */
    }

    /* Second fork to create the second child (only the parent gets here) */
    pid2 = fork();
    if (pid2 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {
        manage_child2(); /* never returns */
    }

    /* Parent process waits for both children to finish */
    if (waitpid(pid1, NULL, 0) == -1) {
        perror("waitpid");
    }
    if (waitpid(pid2, NULL, 0) == -1) {
        perror("waitpid");
    }

    return EXIT_SUCCESS;
}
