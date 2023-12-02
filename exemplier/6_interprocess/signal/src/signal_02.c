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
#include <unistd.h>    /* fork(), ...  */
#include <signal.h>    /* sigaction() */
#include <string.h>    /* memset() */

/**
 * @file signal_02.c
 *
 * A simple program that uses POSIX signals and handles SIGALRM.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2011-12-01
 */

#define DURATION 5

/**
 * @brief Defines the new handler of the SIGALRM signal.
 * @param signal Number of the signal
 */
void handle_alarm(int signal)
{
    if (signal == SIGALRM) {
        printf("\nToo late!\n");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    struct sigaction action;
    int value;
    int remaining_time;
    int result;

    /* Clean up the structure before using it */
    memset(&action, '\0', sizeof(action));

    /* Set the new handler */
    action.sa_handler = &handle_alarm;

    /* Install the new handler of the SIGALRM signal */
    sigaction(SIGALRM, &action, NULL);

    printf("You have %d seconds to enter a number: ", DURATION);
    /* The OS will send an alarm signal to the process in 'DURATION' sec. */
    alarm(DURATION);

    /* Wait for the user to enter a value */
    result = scanf("%d", &value);
    if (result < 1) {
        printf("Failed to read a valid number.\n");
        return EXIT_FAILURE;
    }

    /* Deactivate the sending of an alarm signal by the OS */
    remaining_time = alarm(0);
    printf("\nYou entered the number %d with %d seconds remaining.\n", value, remaining_time);

    return EXIT_SUCCESS;
}
