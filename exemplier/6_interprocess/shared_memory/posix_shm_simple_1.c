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
 * @author Alain Lebret
 * @version	1.0
 * @date 2017-06-08
 */

/**
 * @file posix_shm_simple_1.c
 *
 * Example using parent and child processes sharing memory.
 * Link with \c -lrt.
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

#define SHM_SIZE 100

int main(void)
{
	int fd;
	int i;
	int *ptr;
	pid_t pid;

	srand(time(NULL));

	fd = shm_open("/pipeautique1", O_CREAT | O_RDWR, 0644);
	printf("shm_open returned %d (%d: %s)\n", fd, errno, strerror(errno));

	ftruncate(fd, SHM_SIZE);

	ptr = (int *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("mmap returned %p (%d: %s)\n", ptr, errno, strerror(errno));

	pid = fork();

	if (pid > 0) {
		for (i = 0; i < SHM_SIZE; i++) {
			ptr[i] = rand() % SHM_SIZE;
			printf("%d ", ptr[i]);
		}
		printf("\n");
	} else {
		for (i = 0; i < SHM_SIZE; i++) {
			printf("%d ", ptr[i]);
		}
		printf("\n");
	}
	munmap(ptr, SHM_SIZE);
	if (pid > 0) {
		shm_unlink("/pipeautique1");		
	}
	exit(EXIT_SUCCESS);
}
