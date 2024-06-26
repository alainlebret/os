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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* getpid() and getpgrp() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait(), WIFEXITED and WEXITSTATUS */
#include <errno.h>     /* perror() */

/**
 * @file process_08.c
 *
 * A simple program about a process that executes Gtk windows through his childs.
 */

static char *path = "./moving_window";

/*
 * This block will be executed by the first son
 */
void manage_son1() {
    /* Arguments for the GTK application */
    char *args[] = {"moving_window", "100", "100", "#5bccc9", NULL};

    /* Execute the GTK application */
    execvp(path, args);

    /* If execvp() fails */
    perror("execvp failed for son 1");
    exit(EXIT_FAILURE);
}

/*
 * This block will be executed by the second son
 */
void manage_son2() {
    /* Arguments for the GTK application */
    char *args[] = {"moving_window", "350", "100", "#bca850", NULL};

    /* Execute the GTK application */
    execvp(path, args);

    /* If execvp() fails */
    perror("execvp failed for son 2");
    exit(EXIT_FAILURE);
}

int main(void) {
    pid_t pid1, pid2;

    /* First fork to create the first son */
    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        manage_son1();
    }

    /* Second fork to create the second son */
    if (pid1 > 0) {
        pid2 = fork();

            if (pid2 < 0) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }

            if (pid2 == 0) {
                manage_son2();
            }
    }

    /* Parent process waits for both sons to finish */
    if (pid1 > 0 && pid2 > 0) {
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
    }

    return EXIT_SUCCESS;
}
