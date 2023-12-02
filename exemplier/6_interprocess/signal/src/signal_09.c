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
 * @author Alain Lebret
 * @version	1.0
 * @date 2022-04-12
 */

#define FOREVER for (;;)

void hdl(int signal, siginfo_t *siginfo, void *context)
{
    if (signal == SIGTERM) {
        printf("Sending PID: %ld, UID: %ld\n", (long) siginfo->si_pid, (long) siginfo->si_uid);
    }
}

void sigint_handler(int signal)
{
    if (signal == SIGINT) {
        printf("SIGINT received, exiting.\n");
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    struct sigaction action;
	struct sigaction sigint_action;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));
    /* Use the sa_sigaction field because the handles has two additional parameters */
    action.sa_sigaction = &hdl;
    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGTERM, &action, NULL) < 0) {
        perror("Error using sigaction for SIGTERM");
        exit(EXIT_FAILURE);
    }

	/* Normal signal handler installation for SIGINT */
    memset(&sigint_action, '\0', sizeof(sigint_action));
    sigint_action.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sigint_action, NULL) < 0) {
        perror("Error using sigaction for SIGINT");
        exit(EXIT_FAILURE);
    }

    FOREVER {
        sleep(10);
    }

    return EXIT_SUCCESS;
}
