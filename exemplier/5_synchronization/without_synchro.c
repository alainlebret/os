#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SHM_SIZE sizeof(int)

int ended = 0;

void critical_section(int *value)
{
    if (*value == 0) {
        ended = 1;
    } else {
        *value--;
        fprintf(stdout, "%d has decremented value to: %d\n", getpid(), *value);
    }
}

void random_delay(int at_least_microsecs, int at_most_microsecs)
{
    long choice;
    int range;

    range = at_most_microsecs - at_least_microsecs;
    choice = random(); 
    sleep(at_least_microsecs + choice % range);
}

int main(void) 
{
	int fd;
	int *ptr;
	pid_t pid;

	srand(time(NULL));

	fd = shm_open("/blabla", O_RDWR | O_CREAT, 0666);
	printf("shm_open() returned %d (errno: %d / %s)\n", fd, errno, strerror(errno));

	ftruncate(fd, SHM_SIZE);

	ptr = (int *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("mmap') returned %p (errno: %d / %s)\n", ptr, errno, strerror(errno));

    *ptr = 20;

    pid = fork(); 

    while (!ended) {
        critical_section(ptr);
        random_delay(1, 50);
    }
	
	munmap(ptr, SHM_SIZE);
	if (pid > 0) {
		shm_unlink("/blabla");		
	}

	exit(EXIT_SUCCESS);
}
