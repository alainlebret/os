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
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

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
 * with \em -g and run valgrind.
 */

#include <stdio.h>  /* pour printf() */
#include <stdlib.h> /* pour random(), exit() et execl()*/
#include <unistd.h> /* pour fork() */
#include <sys/types.h> /* pour pid_t et mkfifo() */

#define EVER ;;

int main(void) {
	int *pointer;
	int value;

	pointer = (int *) malloc(1);

	for (EVER) {
		value = random();
		*pointer = value;
	}

	exit(EXIT_SUCCESS); /* unreachable code */
}
