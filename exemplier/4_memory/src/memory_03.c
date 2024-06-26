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

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* random(), exit() */
#include <unistd.h>    /* fork(), sleep() */
#include <sys/types.h> /* pid_t */
#include <signal.h>    /* signal handling */
#include <math.h>      /* sqrt() - link with -lm */

/**
 * @file memory_03.c
 *
 * This program continuously calculates the square root of random numbers,
 * demonstrating the use of the math library (libm) and allowing for the
 * inspection of its memory mapping. Use the following command to check 
 * memory usage:
 * \code{bash}
 * cat /proc/<PID>/maps
 * \endcode
 * 
 * Use Ctrl-C to gracefully exit the program.
 */

void handle_signal(int sig) {
    printf("\nSignal %d received. Exiting now...\n", sig);
    exit(EXIT_SUCCESS);
}

int main(void) {
    struct sigaction sa;
    long number;
    double square_root;

    /* Setup the sigaction struct */
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Set up SIGINT / Ctrl-C for graceful termination */
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Process with PID %d is calculating square roots. Check memory mapping with `cat /proc/%d/maps`\n", getpid(), getpid());

    while (1) {
        number = random();
        square_root = sqrt(number);
        printf("Square root of %ld is %f\n", number, square_root);
        sleep(10); 
    }

    /* Unreachable: the process exits via signal */
}

