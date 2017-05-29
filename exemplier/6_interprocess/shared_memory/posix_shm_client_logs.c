/*
 * posix_shm_client.c
 *
 * Write strings for printing in POSIX shared memory object
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
#include <time.h>
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
	sem_t *mutex_sem,
		*buffer_count_sem,
		*spool_signal_sem;
	int fd_shm;
	char buf[200];
	char *cp;

	/* mutual exclusion semaphore, mutex_sem */
	if ((mutex_sem = sem_open(SEM_MUTEX_NAME, 0, 0, 0)) == SEM_FAILED)
		error("sem_open");
	/* Get shared memory */
	if ((fd_shm = shm_open(SHARED_MEM_NAME, O_RDWR, 0)) == -1)
		error("shm_open");
	if ((shared_mem_ptr = mmap(NULL, sizeof(struct shared_memory),
				   PROT_READ
				   | PROT_WRITE, MAP_SHARED, fd_shm, 0))
	    ==
	    MAP_FAILED)
		error("mmap");

	/* counting semaphore, indicating the number of available buffers. */
	if ((buffer_count_sem = sem_open(SEM_BUFFER_COUNT_NAME, 0, 0, 0)) ==
	    SEM_FAILED)
		error("sem_open");

	/* counting semaphore, indicating the number of strings to be printed. Initial value = 0 */
	if ((spool_signal_sem = sem_open(SEM_SPOOL_SIGNAL_NAME, 0, 0, 0)) ==
	    SEM_FAILED)
		error("sem_open");

	printf("Please type a message: ");
	while (fgets(buf, 198, stdin)) {
		/* remove newline from string */
		int length = strlen(buf);
		if (buf[length - 1] == '\n')
			buf[length - 1] = '\0';

		/* get a buffer: P (buffer_count_sem); */
		if (sem_wait(buffer_count_sem) == -1)
			error("sem_wait: buffer_count_sem");
		/* There might be multiple producers. We must ensure that only one producer uses buffer_index at a time. */

		/* P (mutex_sem); */
		if (sem_wait(mutex_sem) == -1)
			error("sem_wait: mutex_sem");

		/* Critical section */
		time_t now = time(NULL);
		cp = ctime(&now);
		int len = strlen(cp);
		if (*(cp + len - 1) == '\n')
			*(cp + len - 1) = '\0';
		sprintf (shared_mem_ptr->buf[shared_mem_ptr->buffer_index], "%d: %s %s\n",
			 getpid(), cp, buf);

		(shared_mem_ptr->buffer_index)++;
		if (shared_mem_ptr->buffer_index == MAX_BUFFERS)
			shared_mem_ptr->buffer_index = 0;

		/* Release mutex sem: V (mutex_sem) */
		if (sem_post(mutex_sem) == -1)
			error("sem_post: mutex_sem");

		/* Tell spooler that there is a string to print: V (spool_signal_sem); */
		if (sem_post(spool_signal_sem) == -1)
			error("sem_post: (spool_signal_sem");

		printf("Please type a message: ");
	}
	if (munmap(shared_mem_ptr, sizeof(struct shared_memory)) == -1)
		error("munmap");
	exit(EXIT_SUCCESS);
}

/* Print system error and exit */
void error(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
