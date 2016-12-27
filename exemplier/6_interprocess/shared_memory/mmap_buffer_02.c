/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * Chapter "Interprocess communication" / Chapitre "Communication interprocessus"
 *
 * Copyright (C) 1995-2016 Alain Lebret (alain.lebret@ensicaen.fr)
 *
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret (2011)
 * @author Janet Davis (2006)
 * @author Henry Walker (2004)
 * @version	1.3
 * @date 2011-12-01
 */

/**
 * @file mmap_buffer_02.c
 *
 * Producer-consumer program using a shared memory that stores a buffer of
 * integers. This code is based on the example presented by:  Janet Davis (2006)
 * and Henry Walker (2004).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> /* necessary for wait */
#include <sys/wait.h> /* necessary for wait */
#include <sys/mman.h>

#define INTEGER_SIZE sizeof(int)
#define BUFFER_SIZE 5
#define MEMORY_SIZE (BUFFER_SIZE+2)*INTEGER_SIZE
#define ITERATIONS 10

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Writes the square of a serie of integers onto the shared memory.
 */
void write_memory(int *buffer, int *begin, int *end) {
	int i;

	for (i = 0; i < ITERATIONS; i++) {
		while ((*begin + 1) % BUFFER_SIZE == *end);
		buffer[*begin] = i * i;
		*begin = (*begin + 1) % BUFFER_SIZE;
		printf("Parent: initial value %2d before writing in the buffer\n", i);
	}
}

/**
 * Manages the parent process. It writes data to the shared memory and waits
 * for his child to finish.
 */
void manage_parent(int *buffer, int *begin, int *end) {
	pid_t child;
	int status;

	printf("Parent process (PID %d)\n", getpid());
	write_memory(buffer, begin, end);
	printf("Parent: end of production.\n");

	child = wait(&status);
	if (WIFEXITED(status)) {
		printf("Parent: child %d has finished (code %d)\n", child,
				 WEXITSTATUS(status));
	}
}

/**
 * Reads a serie of integers from the shared memory and displays them.
 */
void read_memory(int *buffer, int *in, int *out) {
	int i;
	int value;

	for (i = 0; i < ITERATIONS; i++) {
		sleep(1);  /* waiting for the memory update (not as good as semaphore) */
		while (*in == *out);
		value = buffer[*out];
		*out = (*out + 1) % BUFFER_SIZE;
		printf("Child: element %2d == %2d read from the buffer.\n", i, value);
	}
}

/**
 * Manages the child process that reads all data from shared memory.
 */
void manage_child(int *buffer, int *in, int *out) {
	printf("Child process (PID %d)\n", getpid());
	read_memory(buffer, in, out);
	printf("Child: memory has been consumed.\n");
}

/**
 * Creates and initializes a new shared memory using mmap.
 * @return Pointer on the shared memory
 */
void *create_shared_memory() {
	/* initialize shared memory using mmap */
	void *shared_memory = mmap(0, /* beginning (starting address is ignored) */
										MEMORY_SIZE, /* size of the shared memory */
										PROT_READ | PROT_WRITE, /* protection */
										MAP_SHARED | MAP_ANONYMOUS,
										-1, /* the shared memory do not use a file */
										0);  /* ignored: set when using a file */

	if (shared_memory == (void *) -1) {
		handle_fatal_error("Error allocating shared memory using mmap!\n");
	}
	return shared_memory;
}

int main(void) {
	pid_t pid;

	void *shared_memory; /* shared memory base address */

	int *buffer; /* logical base address for buffer */
	int *in; /* pointer to logical 'in' address for producer */
	int *out; /* pointer to logical 'out' address for consumer */

	shared_memory = create_shared_memory();

	/* The segment of shared memory is organised as:
	 *  0                                                n-1   n   n+1
	 * ----------------------------------------------------------------
	 * |                                                    |    |    |
	 * ----------------------------------------------------------------
	 *  ^                                                    ^    ^
	 *  buffer                                               in   out
	 */

	buffer = (int *) shared_memory;
	in = (int *) shared_memory + BUFFER_SIZE * INTEGER_SIZE;
	out = (int *) shared_memory + (BUFFER_SIZE + 1) * INTEGER_SIZE;

	*in = *out = 0;          /* starting index */

	if (-1 == (pid = fork())) {
		handle_fatal_error("Error using fork()\n");
	}
	if (0 == pid) {
		manage_child(buffer, in, out);
	} else {
		manage_parent(buffer, in, out);
	}

	exit(EXIT_SUCCESS);
}
