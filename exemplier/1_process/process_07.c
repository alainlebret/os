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

#include <stdint.h>    /* C99 int types */
#include <inttypes.h>  /* C99 int types */
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* getpid() and getpgrp() */
#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait(), WIFEXITED and WEXITSTATUS */

/**
 * @file process_07.c
 *
 * A simple program about a process and its group.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.1
 * @date 2017-12-31
 */

/**
 * @brief Handles a fatal error and exit. 
 *
 * It displays the given error message, then exits.
 * @param msg The error message to display before exiting.
 */
void handle_fatal_error_and_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief Manages the parent process. 
 *
 * The parent is waiting for his child to exit.
 */
void manage_parent()
{
    pid_t child;
    int32_t status;

    printf("Parent process: %"
    PRId32
    "\n", getpid());
    printf("Parent group: %"
    PRId32
    "\n", getpgrp());

    child = wait(&status);

    if (WIFEXITED(status)) {
        printf("%d : child %"
        PRId32
        " has finished is work (code: %"
        PRId32
        ")\n",
                getpid(), child, WEXITSTATUS(status));
    }
}

/**
 * @brief Manages the child process by just displaying its PID and group. 
 */
void manage_child()
{
    printf("Child process: %"
    PRId32
    "\n", getpid());
    printf("Child group: %"
    PRId32
    "\n", getpgrp());
}

int main(void)
{
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        handle_fatal_error_and_exit("Error using fork().\n");
    }

    if (pid > 0) {
        manage_parent();
    } else {
        manage_child();
    }

    return EXIT_SUCCESS;
}
