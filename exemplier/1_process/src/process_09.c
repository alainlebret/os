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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * @file process_09.c
 *
 * @brief Another simple program that uses execvp() to executes different
 * applications with arguments.
 */

/**
 * Launches the given program with its given arguments.
 *
 * @param path The path of the program to launch.
 * @param args The arguments to pass to the program.
 */
void launch_process(const char *path, char *const args[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(path, args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
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
    launch_process("/usr/bin/firefox", firefox_args1);
    /* child 2 executes firefox with firefox_args2 */
    launch_process("/usr/bin/firefox", firefox_args2);
    /* child 3 executes gedit with gedit_args */
    launch_process("/usr/bin/gedit", gedit_args);
    /* child 4 executes VLC with vlc_args */
    launch_process("/usr/bin/vlc", vlc_args);

    // Wait for all children to exit
    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
