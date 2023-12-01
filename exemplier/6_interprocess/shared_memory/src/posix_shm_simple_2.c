/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2023 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>


/**
 * @file posix_shm_simple_2.c
 *
 * Another example using parent and child processes sharing memory.
 * Link with \c -lrt.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2017-06-08
 */

typedef struct memory {
    int *table;
} memory_t;

/**
 * Displays content of the specified integer values.
 */
void display(char *prog, int *bytes, int n);

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message);

int main(void)
{
    int shm_fd;     /* file descriptor, from shm_open() */
    int *shm_base;  /* base address, from mmap() */
    int *ptr;       /* shm_base is fixed, ptr is movable */
    memory_t mem;   /* the shared memory */

    mem.table = (int *) malloc(3 * sizeof(int));
    const char *name = "/pipeautique2"; /* shared memory name */
    const int SIZE = sizeof(mem); /* shared memory size */

    /* create the shared memory segment as if it was a file */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0644);
    if (shm_fd == -1) {
        handle_error("Shared memory failed: %s\n");
    }

    /* configure the size of the shared memory segment */
    ftruncate(shm_fd, SIZE);

    /* map the shared memory segment to the address space of the process */
    shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_base == MAP_FAILED) {
        handle_error("Map failed: %s\n");
    }

    int pid = fork();

    if (pid < 0) {
        handle_error("Shared memory failed: %s\n");
    }
    if (pid > 0) { /* parent process */
        display("prod1", shm_base, 3);
        shm_base[0] = 1;
        display("prod1", shm_base, 3);
        sleep(1);
        display("prod1", shm_base, 3);
        shm_base[1] = 3;
        display("prod1", shm_base, 3);
        sleep(1);
        display("prod1", shm_base, 3);
        shm_base[2] = 5;
        display("prod1", shm_base, 3);
        sleep(1);
    } else { /* child process */
        display("prod2", shm_base, 3);
        shm_base[0] = 2;
        display("prod2", shm_base, 3);
        sleep(1);
        display("prod2", shm_base, 3);
        shm_base[1] = 4;
        display("prod2", shm_base, 3);
        sleep(1);
        display("prod2", shm_base, 3);
        shm_base[2] = 6;
        display("prod2", shm_base, 3);
        sleep(1);
    }

    free(mem.table);
    /* remove the mapped memory segment from the address space of the process */
    if (munmap(shm_base, SIZE) == -1) {
        handle_error("Error [unmap()]: ");
    }

    /* close the shared memory segment as if it was a file */
    if (close(shm_fd) == -1) {
        handle_error("Error prod / close() failed. ");
    }

    if (pid > 0) {
        shm_unlink("/pipeautique2");
    }

    return EXIT_SUCCESS;
}

void handle_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}

void display(char *prog, int *values, int n)
{
    int i;

    printf("Display: %s\n", prog);
    for (i = 0; i < n; ++i) {
        printf("%d ", values[i]);
    }
    printf("\n");
}
