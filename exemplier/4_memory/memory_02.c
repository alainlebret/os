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
 * @file memory_02.c
 *
 * Memory mapping of a simple process.
 * Just run the program and verify its memory mapping using the command:
 * \code{bash}
 * cat /proc/<PID>/maps
 * \endcode
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork() */
#include <sys/types.h> /* pid_t and mkfifo() */
#include <time.h>

#define EVER ;;

int main(void) {
	time_t end  = time(NULL) + 240; /* about 4 min */

	printf("Process with PID %d\n", getpid());
	for (EVER) {
		if (time(NULL) >= end) {
			break;
		}
	}

	exit(EXIT_SUCCESS);
}
