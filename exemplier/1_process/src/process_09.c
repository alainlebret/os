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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

/**
 * @file process_09.c
 *
 * @brief Another simple program that uses execvp() to executes different
 * applications with arguments.
 */

/**
 * Launches the given program with its given arguments.
 *
 * @param args The program name (args[0], searched in PATH) followed by its
 *             arguments, terminated by NULL.
 */
void launch_process(char *const args[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args); /* args[0] is searched in PATH */
        perror("execvp failed");
        _exit(127);
    } else if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    char *firefox_args1[] = {"firefox", "-url", "https://foad.ensicaen.fr",
                             "-new-tab", "-url", "https://gitlab.ecole.ensicaen.fr", NULL};
    char *firefox_args2[] = {"firefox", "--search", "chatgpt", NULL};
    char *gedit_args[] = {"gedit", "pointeurs_et_cie1.c", NULL};
    char *vlc_args[] = {"vlc", "resources/mister_trololo.mp4", NULL};

    /* child 1 executes firefox with firefox_args1 */
    launch_process(firefox_args1);
    /* child 2 executes firefox with firefox_args2 */
    launch_process(firefox_args2);
    /* child 3 executes gedit with gedit_args */
    launch_process(gedit_args);
    /* child 4 executes VLC with vlc_args */
    launch_process(vlc_args);

    /* Wait for all children: wait() returns -1 (errno ECHILD) when none is left */
    while (wait(NULL) > 0) {
        ;
    }

    return EXIT_SUCCESS;
}
