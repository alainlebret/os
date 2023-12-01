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

#define FOREVER for (;;)

/**
 * @file memory_04.c
 *
 * Memory mapping of a process using a heap.
 * Just run the program and verify its memory mapping using the command:
 * \code{bash}
 * cat /proc/<PID>/maps
 * \endcode
 *
 * This example also highlights the memory leak problem. Compile the program
 * using -g and run valgrind.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2011-12-01
 */

int main(void)
{
    int *pointer;
    int value;

    pointer = (int *) malloc(sizeof(int));

    FOREVER {
        value = random();
        *pointer = value;
    }

    /* Unreachable: use <Ctrl-C> to exit */
}

