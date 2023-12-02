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

/**
 * @file signal_08a.c
 *
 * A simple program to handle the SIGUSR1 signal. 
 * Open another terminal while running this program and execute:
 * kill -s SIGUSR1 <PID>
 * What if using the following command?
 * kill -s SIGINT <PID> 
 *
 * @author Alain Lebret
 * @version	1.1
 * @date 2022-04-08
 */

volatile sig_atomic_t got_usr1;

/**
 * @brief Handles the signal SIGUSR1.
 */
void sigusr1_handler(int signal)
{
    got_usr1 = 1;
}

/**
 * @brief Handles the signal SIGINT.
 */
void sigint_handler(int signal)
{
    printf("SIGINT received, exiting.\n");
    exit(EXIT_SUCCESS);
}

int main(void)
{
    struct sigaction action_usr1;
	struct sigaction action_int;

    got_usr1 = 0;

    /* Set the new handler */
    action_usr1.sa_handler = sigusr1_handler;
    /* Clear the flags (another way to do a memset) */
    action_usr1.sa_flags = 0;
    /* Clear the mask */
    sigemptyset(&action_usr1.sa_mask);

    /* Same for SIGINT */
    action_int.sa_handler = sigint_handler;
    action_int.sa_flags = 0;
    sigemptyset(&action_int.sa_mask);

    if (sigaction(SIGUSR1, &action_usr1, NULL) == -1) {
        perror("Error using sigaction for SIGUSR1");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGINT, &action_int, NULL) == -1) {
        perror("Error using sigaction for SIGINT");
        exit(EXIT_FAILURE);
    }

    while (!got_usr1) {
        printf("PID %d: working hard...\n", getpid());
        sleep(1);
    }

    printf("Done by SIGUSR1!\n");

    return EXIT_SUCCESS;
}
