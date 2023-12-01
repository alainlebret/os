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
#include <stdlib.h>    /* random(), exit() and execl()*/
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t and mkfifo() */
#include <math.h>      /* sqrt() -- don't forget to link with -lm */

#define FOREVER for(;;)

/**
 * @file memory_03.c
 *
 * This program continuously calculates the square root of random numbers, 
 * allowing for inspection of its memory mapping, particularly to observe 
 * the use of the math library (libm).
 * Verify its memory mapping using the command:
 * \code{bash}
 * cat /proc/<PID>/maps
 * \endcode
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2011-12-01
 */

int main(void)
{
    long number;
    double square_root;

    printf("Process with PID %d\n", getpid());

    FOREVER {
        number = random();
        square_root = sqrt(number);
    }

    /* Unreachable: use <Ctrl-C> to exit */

}
