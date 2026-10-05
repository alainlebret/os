/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <sys/wait.h>

/**
 * @file posix_shm_simple_1.c
 *
 * Example using parent and child processes sharing memory without synchronization.
 * Link with \c -lrt under Linux.
 *
 * No synchronization on purpose: the child will probably display 0s, read
 * before the parent has written.
 */

#define SHM_SIZE 100

int main(void) {
    int fd;
    int i;
    int *ptr;
    pid_t pid;
    const size_t shm_bytes = SHM_SIZE * sizeof(int);

    srand(time(NULL));

    fd = shm_open("/pipeautique1", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Error [shm_open()]");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, (off_t) shm_bytes) == -1) {
        perror("Error [ftruncate()]");
        exit(EXIT_FAILURE);
    }

    ptr = (int *) mmap(NULL, shm_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Error [mmap()]");
        exit(EXIT_FAILURE);
    }
    close(fd);

    pid = fork();
    if (pid == -1) {
        perror("Error [fork()]");
        exit(EXIT_FAILURE);
    }

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
    munmap(ptr, shm_bytes);
    if (pid > 0) {
        wait(NULL);
        shm_unlink("/pipeautique1");
    }

    return EXIT_SUCCESS;
}
