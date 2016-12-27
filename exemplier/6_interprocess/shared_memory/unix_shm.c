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
 * @author Alain Lebret
 * @version	1.0
 * @date 2016-11-01
 */

/**
 * @file posix_semaphore.c
 *
 * Example using child processes sharing memory using POSIX semaphore.
 * Link with \c -lpthread.
 */

#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <fcntl.h>          /* for O_* constants */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */

int *shared_value; /* shared variable */
sem_t *sem;        /* synch semaphore */
int shmid;         /* shared memory id */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char *msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

/**
 * Initializes the shared memory and semaphore's value.
 * @return The number of child processes
 */
int initialize(){
	unsigned int number_children; /* fork count */
	key_t shmkey;                 /* shared memory key */
	unsigned int sem_value;       /* semaphore value */

	/* initialize a shared variable in shared memory */
	shmkey = ftok("/dev/null", 5);  /* valid directory name and a number */
	printf("shmkey for shared value = %d\n", shmkey);

	shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT);
	if (shmid < 0){  /* shared memory error check */
		handle_fatal_error("Error trying to allocate shared memory\n");
	}

	shared_value = (int *)shmat(shmid, NULL, 0); /* attach shared_value to memory */
	*shared_value = 0;
	printf("shared value=%d is allocated in shared memory.\n\n", *shared_value);

	printf("How many children do you want to fork:\n");
	scanf("%u", &number_children);

	printf("Enter a semaphore value: ");
	scanf("%u", &sem_value);

	/* initialize semaphores for shared processes */
	sem = sem_open("pSem", O_CREAT | O_EXCL, 0644, sem_value);
	/* name of semaphore is "pSem", semaphore is reached using this name */
	sem_unlink("pSem");
	/* unlink prevents the semaphore existing forever */
	/* if a crash occurs during the execution         */
	printf("semaphores initialized.\n\n");

	return number_children;
}

/**
 * Manages the parent process. Parent is waiting for his child.
 */
void manage_parent(int shmid) {
	pid_t child;

	printf("Parent process (PID %d)\n", getpid());

	/* wait for all children to exit */
	child = waitpid(-1, NULL, 0);
	while (child){
		if (errno == ECHILD)
			break;
		child = waitpid(-1, NULL, 0) > 0;
	}
	printf("\nParent: All children have exited.\n");

	/* shared memory detach */
	shmdt(shared_value);
	shmctl(shmid, IPC_RMID, 0);

	/* cleanup semaphores */
	sem_destroy(sem);
}

/**
 * Manages the child process. Child enters critical section and modifies the
 * shared value.
 */
void manage_child(int child_number) {
	printf("Child process (PID %d)\n", getpid());
	sem_wait(sem);           /* P operation */
	printf("Child (PID %d) is in critical section.\n", child_number);
	sleep(1);
	*shared_value += child_number % 3;  /* increment by 0, 1 or 2 based on number */
	printf("Child (PID %d): new value = %d.\n", child_number, *shared_value);
	sem_post(sem);           /* V operation */
	printf("Child (PID %d) gets out of critical section.\n", child_number);
}

int main(void) {
	pid_t pid;
	int number_children; /* fork count */
	int child_number;

	number_children = initialize();

	/* fork child processes */
	for (child_number = 0; child_number < number_children; child_number++){
		pid = fork();
		if (pid < 0) {
			handle_fatal_error("Error using fork().\n");
		}
		else if (pid == 0)
			break;  /* child processes */
	}

	if (pid > 0) {
		manage_parent(shmid);
	} else {
		manage_child(child_number);
	}

	exit(EXIT_SUCCESS);
}
