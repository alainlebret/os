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
#include <errno.h>
#include <signal.h>
#include <string.h>

/**
 * @file signal_09.c
 *
 * A simple program that setups a signal handler with 3 arguments, including
 * siginfo_t.
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

#define FOREVER for (;;)

void hdl(int signal, siginfo_t *siginfo, void *context)
{
    printf("Sending PID: %ld, UID: %ld\n", (long) siginfo->si_pid, (long) siginfo->si_uid);
}

int main(int argc, char *argv[])
{
    struct sigaction action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));

    /* Use the sa_sigaction field because the handles has two additional parameters */
    action.sa_sigaction = &hdl;

    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGTERM, &action, NULL) < 0) {
        perror("Error using sigaction");
        exit(EXIT_FAILURE);
    }

    FOREVER {
        sleep(10);
    }

    /* Unreachable: use <Ctrl-C> to exit */

}
