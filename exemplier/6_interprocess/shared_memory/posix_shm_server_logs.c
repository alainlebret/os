/*
 * posix_shm_server.c
 *
 * Write strings in POSIX shared memory to file (Server process)
 *
 * Copyright (c) 2007-2016 SoftPrayog.in. All Rights reserved.
 *
 * <https://www.softprayog.in/programming/interprocess-communication-using-posix-shared-memory-in-linux>
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

/* Buffer data structures */
#define MAX_BUFFERS 10
#define LOGFILE "/tmp/example.log"
#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_BUFFER_COUNT_NAME "/sem-buffer-count"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"
#define SHARED_MEM_NAME "/posix-shared-mem-example"

struct shared_memory {
	char buf[MAX_BUFFERS][256];
	int buffer_index;
	int buffer_print_index;
};

void error(char *msg);

int main(int argc, char **argv)
{
	struct shared_memory *shared_mem_ptr;
	sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;
	int fd_shm, fd_log;
	char mybuf[256];

	/* Open log file */
	if ((fd_log = open(LOGFILE,
		O_CREAT | O_WRONLY | O_APPEND | O_SYNC,
		0666)) == -1)
		error("fopen");

	/* mutual exclusion semaphore, mutex_sem with an initial value 0. */
	if ((mutex_sem = sem_open(SEM_MUTEX_NAME, O_CREAT, 0660, 0))
	    == SEM_FAILED)
		error("sem_open");

	/* Get shared memory */
	if ((fd_shm = shm_open(SHARED_MEM_NAME, O_RDWR | O_CREAT, 0660)) == -1)
		error("shm_open");

	if (ftruncate(fd_shm, sizeof(struct shared_memory)) == -1)
		error("ftruncate");

	if ((shared_mem_ptr = mmap(NULL,
		sizeof(struct shared_memory),
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fd_shm,
		0)) == MAP_FAILED)
		error("mmap");

	/* Initialize the shared memory */
	shared_mem_ptr->buffer_index = shared_mem_ptr->buffer_print_index = 0;

	/* counting semaphore, indicating the number of available buffers. Initial value = MAX_BUFFERS */
	if ((buffer_count_sem = sem_open(SEM_BUFFER_COUNT_NAME,
		O_CREAT,
		0660,
		MAX_BUFFERS)) == SEM_FAILED)
		error("sem_open");

	/* counting semaphore, indicating the number of strings to be printed. Initial value = 0 */
	if ((spool_signal_sem = sem_open(SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0660, 0))
	    ==
	    SEM_FAILED)
		error("sem_open");

	/* Initialization complete; now we can set mutex semaphore as 1 to indicate shared memory segment is available */
	if (sem_post(mutex_sem) == -1)
		error("sem_post: mutex_sem");

	while (1) { /* forever */
		/* Is there a string to print? P (spool_signal_sem); */
		if (sem_wait(spool_signal_sem) == -1)
			error("sem_wait: spool_signal_sem");

		strcpy(mybuf, shared_mem_ptr->buf[shared_mem_ptr->buffer_print_index]);

		/* Since there is only one process (the logger) using the buffer_print_index, mutex semaphore is not necessary */
		(shared_mem_ptr->buffer_print_index)++;
		if (shared_mem_ptr->buffer_print_index == MAX_BUFFERS)
			shared_mem_ptr->buffer_print_index = 0;

		/* Contents of one buffer has been printed. One more buffer is available for use by producers. Release buffer: V (buffer_count_sem); */
		if (sem_post(buffer_count_sem) == -1)
			error("sem_post: buffer_count_sem");

		/* write the string to file */
		if (write(fd_log, mybuf, strlen(mybuf)) != strlen(mybuf))
			error("write: logfile");
	}
}

/* Print system error and exit */
void error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
