/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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

/**
 * @file memory_06.c
 *
 * This program demonstrates the memory layout of a process in a Unix/Linux
 * environment by printing the addresses of various functions, global 
 * variables, and dynamically allocated memory, illustrating how different 
 * segments (text, data, heap, stack) are organized in memory. It is based
 * on the code by John Shapley Gray.
 *
 * Ref.: John Shapley Gray. "Interprocess Communications in Linux: The Nooks 
 * and Crannies". Prentice Hall. 2003.
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

#define SHOW_ADDRESS(ID, I) printf("The id %s \t\t is at:%8lX\n", ID, (unsigned long int)&I);

/*
 * etext, edata and end are not variables of the program: they are symbols
 * defined by the linker (see man 3 end). Their ADDRESS is the end of the
 * text segment, of the initialized data and of the BSS. macOS does not
 * provide them, but offers get_etext(), get_edata() and get_end().
 */
#ifdef __APPLE__
#include <mach-o/getsect.h>
#define ADDR_ETEXT get_etext()
#define ADDR_EDATA get_edata()
#define ADDR_END   get_end()
#else
extern char etext, edata, end;
#define ADDR_ETEXT ((unsigned long int) &etext)
#define ADDR_EDATA ((unsigned long int) &edata)
#define ADDR_END   ((unsigned long int) &end)
#endif

char *g_pointer = "A string in C"; /* initialized global */
char g_buff[100];                  /* uninitialized global */

void pointer_function(int local_non_init) {
    int local_init;

    local_init = 3;

    SHOW_ADDRESS(" LocUninit (inside PtrFct) ", local_non_init)
    if (local_non_init > 0) {
        pointer_function(local_non_init - local_init);
    }
}

int main(int argc, char *argv[]) {
    int i = 3;                  /* local initialized --> stack segment */
    static long diff;           /* static local uninitialized --> BSS segment */
    static int stack_calls = 5; /* static local initialized --> data segment */

    int *int_ptr1 = (int *) malloc(10 * sizeof(int));  /* heap */
    if (!int_ptr1) {
        perror("Failed to allocate memory for int_ptr1");
        exit(EXIT_FAILURE);
    }
    int *int_ptr2 = (int *) malloc(10 * sizeof(int));  /* heap */
    if (!int_ptr2) {
        perror("Failed to allocate memory for int_ptr2");
        free(int_ptr1);  /* Clean up previous allocation */
        exit(EXIT_FAILURE);
    }

    snprintf(g_buff, sizeof(g_buff), " Layout of virtual memory \n ");

    if (write(1, g_buff, strlen(g_buff)) == -1) {
        perror("Error [write()]");
    }

    printf("Adr etext : %8lX \t\t Adr edata : %8lX \t\t Adr end : %8lX \n",
           ADDR_ETEXT, ADDR_EDATA, ADDR_END);

    printf(" Variable \t\t HEX_ADDR\n ");

    SHOW_ADDRESS(" main function", main);
    SHOW_ADDRESS(" pointer_function() ", pointer_function);
    printf("The id  etext  \t\t is at:%8lX\n", ADDR_ETEXT);
    diff = (long) ((unsigned long int) &pointer_function - (unsigned long int) &main);
    printf(" pointer_function() is  %ld bytes above main\n", diff);


    SHOW_ADDRESS(" Global pointer ", g_pointer);
    diff = (long) ((unsigned long int) &g_pointer - (unsigned long int) &pointer_function);
    printf(" g_pointer is %ld bytes above pointer_function()\n", diff);

    SHOW_ADDRESS(" Global Buff", g_buff);
    printf(" int_ptr1 %8lX\n", (unsigned long int) int_ptr1);
    printf(" int_ptr2 %8lX\n", (unsigned long int) int_ptr2);
    SHOW_ADDRESS(" diff ", diff);
    SHOW_ADDRESS(" stack calls", stack_calls);
    printf("The id  edata  \t\t is at:%8lX\n", ADDR_EDATA);
    printf("The id  end  \t\t is at:%8lX\n", ADDR_END);
    SHOW_ADDRESS(" argc ", argc);
    SHOW_ADDRESS(" argv ", argv);
    SHOW_ADDRESS(" i ", i);
    pointer_function(stack_calls);

    return EXIT_SUCCESS;
}

