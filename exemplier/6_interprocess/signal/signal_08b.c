/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file signal_08b.c
 *
 * A simple program to handle the SIGUSR1 signal, but this time we 
 * mask other signals. 
 * Open another terminal while running this program and execute:
 * kill -s SIGUSR1 <PID>
 * What if using the following command?
 * kill -s SIGINT <PID> 
 */

volatile sig_atomic_t got_usr1;

/**
 * @brief Handles the signal SIGUSR1.
 */
void sigusr1_handler(int signal)
{
    if (signal == SIGUSR1) {
        got_usr1 = 1;
    }
}

int main(void)
{
    struct sigaction action;
	sigset_t the_new_mask;

    got_usr1 = 0;

    /* Initialize the_new_mask to full, including all signals */
	sigfillset(&the_new_mask);
	/* Delete SIGUSR1 from the_new_mask */
	sigdelset(&the_new_mask, SIGUSR1);
	/* Change the signal mask with the_new_mask */
	sigprocmask(SIG_SETMASK, &the_new_mask, NULL);


    /* Set the new handler */
    action.sa_handler = sigusr1_handler;
    /* Clear the flags (another way to do a memset) */
    action.sa_flags = 0;
    /* Apply the_new_mask */
    action.sa_mask = the_new_mask;

    if (sigaction(SIGUSR1, &action, NULL) == -1) {
        perror("Error using sigaction");
        exit(EXIT_FAILURE);
    }

    while (!got_usr1) {
        printf("PID %d: working hard...\n", getpid());
        sleep(1);
    }

    printf("Done by SIGUSR1!\n");

    exit(EXIT_SUCCESS);
}
