/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * "Files, pipes and fifo" / "Fichiers et entrées-sorties"
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
 * @file win32_pipe_child.c
 *
 * A child process under Ms_Windows using an anonymous pipe to receive
 * some data from its parent.
 */

#include <stdio.h> 
#include <windows.h>

#define BUFFER_SIZE 100

int main(void)
{
	HANDLE readHandle;
	char buffer[BUFFER_SIZE];
	DWORD read;

	/*
	 * Gets the read handle of the pipe.
	 */
	readHandle = GetStdHandle(STD_INPUT_HANDLE);

	/*
	 * Reads from the pipe.
	 */
	if (ReadFile(readHandle, buffer, BUFFER_SIZE, &read, NULL)) {
		printf("Child read %s from the pipe.\n", buffer);
	} else {
		fprintf(stderr, "Error reading from the pipe.\n");
	}

	exit(EXIT_SUCCESS);
}
