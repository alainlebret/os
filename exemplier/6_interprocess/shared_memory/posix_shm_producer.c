/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 *
 * modifications by dheller@cse.psu.edu, 31 Jan. 2014
 * http://www.cse.psu.edu/~deh25/cmpsc473/notes/OSC/Processes/shm.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>

void display(char *prog, char *bytes, int n);

void handle_fatal_error(char *msg);

int main(void)
{
	const char *name = "/shm-example"; /* shared memory name */
	const int SIZE = 4096; /* shared memory size */

	const char *message0 = "Studying ";
	const char *message1 = "Operating Systems ";
	const char *message2 = "Is Fun!";
	const char *msg_end = "\n";

	int shm_fd;      /* file descriptor, from shm_open() */
	char *shm_base;  /* base address, from mmap() */
	char *ptr;       /* shm_base is fixed, ptr is movable */

	/* create the shared memory segment as if it was a file */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		handle_fatal_error("prod: Shared memory failed: %s\n");
	}

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);

	/* map the shared memory segment to the address space of the process */
	shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (shm_base == MAP_FAILED) {
		handle_fatal_error("prod: Map failed: %s\n");
	}

	/*
	 * Write to the mapped shared memory region.
	 *
	 * We increment the value of ptr after each write, but we
	 * are ignoring the possibility that sprintf() fails.
	 */
	display("prod", shm_base, 64);
	ptr = shm_base;
	ptr += sprintf(ptr, "%s", message0);
	ptr += sprintf(ptr, "%s", message1);
	ptr += sprintf(ptr, "%s", message2);
	ptr += sprintf(ptr, "%s", msg_end);
	display("prod", shm_base, 64);

	/* remove the mapped memory segment from the address space of the process */
	if (munmap(shm_base, SIZE) == -1) {
		handle_fatal_error("prod: Unmap failed: %s\n");
	}

	/* close the shared memory segment as if it was a file */
	if (close(shm_fd) == -1) {
		handle_fatal_error("prod: Close failed: %s\n");
	}

	exit(EXIT_SUCCESS);
}

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg)
{
	printf(msg, strerror(errno));
	exit(EXIT_FAILURE);
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
