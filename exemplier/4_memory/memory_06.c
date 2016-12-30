/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Memory management" / "Gestion de la mémoire"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file memory_06.c
 *
 * An example using memory.
 *
 * +------------------+
 * |  TEXT            | x		= main()
 * |                  |
 * |  instructions    | x		= pointer_function()
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
 * +------------------+ x	   = end of data segment(s)
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
 * |  STACK           |
 * |  - function      | x         = init local | Instruction pushed
 * |                  | x         = init local | for pointer_function()
 * |                  |
 * |  - local         | x         = loc        | Instruction pushed
 * |    variables     |                        | for main()
 * |                  |
 * +------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SHOW_ADDRESS(ID, I) printf("The id %s \t\t is at:%8X\n", ID, (unsigned long int)&I);

int etext, edata, end;
char *g_pointer = "String in C \n ";   /* initialized global */
char g_buff[100];               /* uninitialized global */


void pointer_function(int local_non_init) {
	int local_init = 1;

	SHOW_ADDRESS(" LocUninit (inside PtrFct) ", local_non_init)
	if (local_non_init > 0) {
		pointer_function(local_non_init - local_init);
	}
}

int main(int argc, char *argv[], char *envp[]) {
	int i = 0;            /* local intialized => stack segment */
	static int diff;      /* static local uninitialized =>data segment */
	static int stack_calls = 5; /* static local intialized => data segment */

	int *int_ptr1 = (int *) malloc(10 * sizeof(int)); /* heap */
	int *int_ptr2 = (int *) malloc(10 * sizeof(int)); /* heap */


	strcpy (g_buff, " Layout of virtual 4_memory \n ");

	write(1, g_buff, strlen(g_buff) + 1);
	printf("Adr etext : %8X \t\t Adr edata : %8X \t\t Adr end : %8X \n",
			 (unsigned long int) &etext, (unsigned long int) &edata, (
					 unsigned long int) &end);

	printf(" Variable \t\t HEX_ADDR\n ");

	SHOW_ADDRESS(" main ", main);
	SHOW_ADDRESS(" pointer_function ", pointer_function);
	SHOW_ADDRESS(" etext ", etext);
	diff = (unsigned long int) &pointer_function - (unsigned long int) &main;
	printf(" pointer_function is  %d bytes above main\n", diff);


	SHOW_ADDRESS(" Global Ptr ", g_pointer);
	diff =
			(unsigned long int) &g_pointer - (unsigned long int) &pointer_function;
	printf(" g_pointer is %d bytes above pointer_function \n", diff);

	SHOW_ADDRESS(" Global Buff", g_buff);
	printf(" int_ptr1 %8X\n", (unsigned long int) int_ptr1);
	printf(" int_ptr2 %8X\n", (unsigned long int) int_ptr2);
	SHOW_ADDRESS(" diff ", diff);
	SHOW_ADDRESS(" stack calls", stack_calls);
	SHOW_ADDRESS(" edata ", edata);
	SHOW_ADDRESS(" end ", end);
	SHOW_ADDRESS(" argc ", argc);
	SHOW_ADDRESS(" argv ", argv);
	SHOW_ADDRESS(" i ", i);
	pointer_function(stack_calls);

	exit(EXIT_SUCCESS);
}
