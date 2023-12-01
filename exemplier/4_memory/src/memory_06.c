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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SHOW_ADDRESS(ID, I) printf("The id %s \t\t is at:%8lX\n", ID, (unsigned long int)&I);

/**
 * @file memory_06.c
 *
 * Example using memory. It is based on an example from the following book:
 * John Shapley Gray. "Interprocess Communications in Linux: The Nooks and 
 * Crannies". Prentice Hall. 2003.
 *
 * +------------------+
 * |  TEXT            | x     = main()
 * |                  |
 * |  instructions    | x     = pointer_function()
 * |  binary code     |
 * |                  |
 * +------------------+ x     = 'etext'
 * |  DATA            | x     = initialized globals
 * |  - initialized   |
 * |    variables     | x     = initialized static locals
 * |                  |
 * |                  | x     = 'edata'
 * |                  | x     = non-initialized static locals
 * |                  |
 * | ---------------  |
 * |  - non           | x     = 'end'
 * |  initialized     | x     = non-initialized globals
 * |  variables       |
 * |                  |
 * |                  |
 * | ---------------  |
 * |  - heap for      | x     = pointed by 'int_ptr1'
 * |    dynamic       | x		= pointed by 'int_ptr2'
 * |    allocation    |
 * |                  |
 * |                  |
 * +------------------+ x     = end of data segment(s)
 *          |
 *          |
 *          V
 *          .
 *          .
 *          .
 *          ^
 *          |
 *          |
 * +------------------+
 * |      STACK       |
 * |  - function      | x     = init local | Instruction pushed
 * |                  | x     = init local | for pointer_function()
 * |                  |
 * |  - local         | x     = loc        | Instruction pushed
 * |    variables     |                    | for main()
 * |                  |
 * +------------------+
 */

int etext;
int edata;
int end;

char *g_pointer = "A string in C"; /* initialized global */
char g_buff[100];                  /* uninitialized global */

void pointer_function(int local_non_init)
{
    int local_init;

    local_init = 3;

    SHOW_ADDRESS(" LocUninit (inside PtrFct) ", local_non_init)
    if (local_non_init > 0) {
        pointer_function(local_non_init - local_init);
    }
}

int main(int argc, char *argv[], char *envp[])
{
    int i = 3;                  /* local intialized --> stack segment */
    static int diff;            /* static local uninitialized --> data segment */
    static int stack_calls = 5; /* static local intialized --> data segment */

    int *int_ptr1 = (int *) malloc(10 * sizeof(int)); /* heap */
    int *int_ptr2 = (int *) malloc(10 * sizeof(int)); /* heap */

    strcpy(g_buff, " Layout of virtual memory \n ");

    if (write(1, g_buff, strlen(g_buff) + 1) == -1) {
        perror("Error [write()]: ");
    }
     
    printf("Adr etext : %8lX \t\t Adr edata : %8lX \t\t Adr end : %8lX \n",
           (unsigned long int) &etext, (unsigned long int) &edata, (
                   unsigned long int) &end);

    printf(" Variable \t\t HEX_ADDR\n ");

    SHOW_ADDRESS(" main function", main);
    SHOW_ADDRESS(" pointer_function() ", pointer_function);
    SHOW_ADDRESS(" etext ", etext);
    diff = (unsigned long int) &pointer_function - (unsigned long int) &main;
    printf(" pointer_function() is  %d bytes above main\n", diff);


    SHOW_ADDRESS(" Global pointer ", g_pointer);
    diff =
            (unsigned long int) &g_pointer - (unsigned long int) &pointer_function;
    printf(" g_pointer is %d bytes above pointer_function()\n", diff);

    SHOW_ADDRESS(" Global Buff", g_buff);
    printf(" int_ptr1 %8lX\n", (unsigned long int) int_ptr1);
    printf(" int_ptr2 %8lX\n", (unsigned long int) int_ptr2);
    SHOW_ADDRESS(" diff ", diff);
    SHOW_ADDRESS(" stack calls", stack_calls);
    SHOW_ADDRESS(" edata ", edata);
    SHOW_ADDRESS(" end ", end);
    SHOW_ADDRESS(" argc ", argc);
    SHOW_ADDRESS(" argv ", argv);
    SHOW_ADDRESS(" i ", i);
    pointer_function(stack_calls);

    return EXIT_SUCCESS;
}

