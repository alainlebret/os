/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        *value = *value - 1;
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

	if (ftruncate(fd, SHM_SIZE) == -1) {
		perror("ftruncate error");
		exit(EXIT_FAILURE);
	}

	ptr = (int *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("mmap') returned %p (errno: %d / %s)\n", (void *)ptr, errno, strerror(errno));

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

	return EXIT_SUCCESS;
}
