/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 *
 * modifications by dheller@cse.psu.edu, 31 Jan. 2014
 * http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

void display(char *prog, char *bytes, int n);

int main(void)
{
	const char *name = "/shm-example";   // file name
	const int SIZE = 4096;      // file size

	int shm_fd;      // file descriptor, from shm_open()
	char *shm_base;   // base address, from mmap()

	/* open the shared memory segment as if it was a file */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("cons: Shared memory failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* map the shared memory segment to the address space of the process */
	shm_base = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (shm_base == MAP_FAILED) {
		printf("cons: Map failed: %s\n", strerror(errno));
		// close and unlink?
		exit(EXIT_FAILURE);
	}

	/* read from the mapped shared memory segment */
	display("cons", shm_base, 64);   // first as bytes, then as a string
	printf("%s", shm_base);

	/* remove the mapped shared memory segment from the address space of the process */
	if (munmap(shm_base, SIZE) == -1) {
		printf("cons: Unmap failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* close the shared memory segment as if it was a file */
	if (close(shm_fd) == -1) {
		printf("cons: Close failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* remove the shared memory segment from the file system */
	if (shm_unlink(name) == -1) {
		printf("cons: Error removing %s: %s\n", name, strerror(errno));
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

void display(char *prog, char *bytes, int n)
{
	int i;

	printf("display: %s\n", prog);
	for (i = 0; i < n; i++) {
		printf("%02x%c", bytes[i], ((i + 1) % 16) ? ' ' : '\n');
	}
	printf("\n");
}
