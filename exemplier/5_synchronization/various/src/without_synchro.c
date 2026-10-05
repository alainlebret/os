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
 * @file without_synchro.c
 *
 * This program demonstrates inter-process communication using shared memory
 * in a Unix-like environment. It creates a shared integer variable and 
 * decrements this value in a critical section until it reaches zero, with
 * both parent and child processes participating in the decrement operation.
 *
 * WARNING: synchronization is deliberately missing. The test and the
 * decrement of the shared value are not atomic, so both processes may read
 * the same value (lost update, the same value displayed twice) or both
 * decrement it when it is 1 (the value becomes negative).
 * See ../../semaphores/src to protect such a critical section.
 */

#define SHM_SIZE sizeof(int)

int ended = 0;

void critical_section(int *value) {
    if (*value <= 0) {      /* <= : the race may make it negative */
        ended = 1;
    } else {
        *value = *value - 1;
        fprintf(stdout, "%ld has decremented value to: %d\n",
                (long) getpid(), *value);
    }
}

void random_delay(int at_least_ms, int at_most_ms) {
    int range = at_most_ms - at_least_ms;
    long ms = at_least_ms + rand() % range;
    struct timespec delay = { ms / 1000, (ms % 1000) * 1000000L };

    nanosleep(&delay, NULL);
}

int main(void) {
    int fd;
    int *ptr;
    pid_t pid;

    fd = shm_open("/blabla", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open error");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate error");
        exit(EXIT_FAILURE);
    }

    ptr = (int *) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap error");
        exit(EXIT_FAILURE);
    }
    close(fd);

    *ptr = 20;

    pid = fork();
    if (pid == -1) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    srand((unsigned int) getpid());  /* different delays in each process */

    while (!ended) {
        critical_section(ptr);
        random_delay(1, 50);
    }

    munmap(ptr, SHM_SIZE);
    if (pid > 0) {
        wait(NULL);         /* collects the child */
        shm_unlink("/blabla");
    }

    return EXIT_SUCCESS;
}
