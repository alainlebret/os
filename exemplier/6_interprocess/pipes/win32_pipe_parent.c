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
 * @file win32_pipe_parent.c
 *
 * A parent process that runs under Ms_Windows using an anonymous pipe to 
 * communicate with its child.
 */

#include <stdio.h>  
#include <stdlib.h> 
#include <windows.h>

#define BUFFER_SIZE 100

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
	fprintf(stderr, msg);
	exit(EXIT_FAILURE);
}

int main(void)
{
	HANDLE readHandle;
	HANDLE writeHandle;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char buffer[BUFFER_SIZE];
	DWORD written;

	/*
	 * Set security attributes allowing pipes to be inherited.
	 */
	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
	
	/*
	 * Allocates memory.
	 */
	ZeroMemory(&pi, sizeof(pi));

	/*
	 * Creates a new anonymous pipe
	 */
	if (!CreatePipe(&readHandle, &writeHandle, &sa, 0) {
		handle_fatal_error("\nError when trying to create the pipe.\n");
	}

	/*
	 * Creates the START_INFO structure for the child process.
	 */
	GetStartupInfo(&si);
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	/*
	 * Redirects the standard input to the read end of the pipe.
	 */
	si.hStdInput = readHandle;
	si.dwFlags = STARTF_USESTDHANDLES;

	/*
	 * Don't allow the child process to inherit the write end of the pipe.
	 */
	SetHandleInformation(writeHandle, HANDLE_FLAG_INHERIT, 0);

	/*
	 * Creates the child process.
	 */
	CreateProcess(NULL, 
		"win32_pipe_child.exe", 
		NULL, 
		NULL, 
		TRUE, 
		/* inherit handles */ 0, 
		NULL, 
		NULL, 
		&si, 
		&pi);

	/*
	 * Closes the unused end of the pipe.
	 */
	CloseHandle(readHandle);
  
	/*
	 * Writes data to the pipe.
	 */
	if (!WriteFile(writeHandle, buffer, BUFFER_SIZE, &written, NULL)) {
		fprintf(stderr, "Error writing to the pipe.\n");
	}

	/*
	 * Wait for the child to exit.
	 */
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	exit(EXIT_SUCCESS);
}
