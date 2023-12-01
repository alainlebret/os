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
#include <stdio.h>    /* printf() */
#include <string.h>   /* memset() */
#include <signal.h>   /* sigaction() */

/**
 * @file signal_05.c
 *
 * A simple program that uses POSIX signals and handles many signals.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

#define FOREVER for (;;)

/**
 * @brief Defines a naive handler to display the received signal number.
 * @param signal Number of the signal.
 */
void handle(int signal)
{
    printf("Signal number %d has been received.\n", signal);
    fflush(stdout);
}

int main(void)
{
    struct sigaction action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));

    /* Set the new handler */
    action.sa_handler = &handle;

    /* We do not block any specific signal */
    sigemptyset(&action.sa_mask);

    /* Three signals will be handled by the process */
    sigaction(SIGINT, &action, 0);
    sigaction(SIGQUIT, &action, 0);
    sigaction(SIGTERM, &action, 0);

    FOREVER {}

    /* Unreachable: use <Ctrl-C> to exit */

}
