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
 * @date 2022-05-23
 */

/**
 * @file posix_shm_simple_3.c
 *
 * Another example using parent and child processes sharing a complex structure memory.
 * Link with \c -lrt.
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

void handle_error(char *message);

#define PAGESIZE 4096

typedef struct vector_t {
    float x;
    float y;
    float z;
} vector;

typedef struct color_t {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} color;

typedef struct s1_t {
    int id;
    char name[20];
    int age;
    char padding[PAGESIZE - 2 * sizeof(int) - 20 * sizeof(char)];
} s1;

typedef struct s2_t {
    vector vec;
    color col;
    char padding[PAGESIZE - sizeof(vector) - 20 * sizeof(color)];
} s2;

typedef struct s1_et_s2_t {
    s1 a_s1;
    s2 a_s2;
} s1_and_s2;


int main(void)
{
    int shm_fd;
    pid_t pid;
    s1 *ptr1;
    s2 *ptr2;

    srand(time(NULL));

    /* create the shared memory segment as if it was a file */
    shm_fd = shm_open("/pipeautique3", O_CREAT | O_RDWR, 0644);
    if (shm_fd == -1) {
        handle_error("Opening shared memory file failed: %s\n");
    }

    ftruncate(shm_fd, sizeof(s1_and_s2));

    /* map the shared memory segment for struct s1 to the address space of the process */
    ptr1 = (s1 *) mmap(NULL, sizeof(s1), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr1 == MAP_FAILED) {
        handle_error("Map failed: %s\n");
    }

    /* map the shared memory segment for struct s2 to the address space of the process */
    ptr2 = (s2 *) mmap(NULL, sizeof(s2), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, sizeof(s1));
    if (ptr2 == MAP_FAILED) {
        handle_error("Map failed: %s\n");
    }

    printf("> %p\n", (void *) ptr1);
    printf("> %p\n", (void *) ptr2);

    pid = fork();

    if (pid < 0) {
        handle_error("Shared memory failed: %s\n");
    }
    if (pid > 0) { /* parent process */
        strcpy(ptr1->name, "Monkeypox");
        ptr2->col.red = 112;
    } else { /* child process */
        printf("%s\n", ptr1->name);
        printf("%i\n", ptr2->col.red);
    }

    /* remove the mapped memory segments from the address space of the process */
    if (munmap(ptr1, sizeof(s1)) == -1) {
        handle_error("Unmap ptr1 failed: %s\n");
    }
    if (munmap(ptr2, sizeof(s2)) == -1) {
        handle_error("Unmap ptr2 failed: %s\n");
    }

    /* close the shared memory segment as if it was a file */
    if (close(shm_fd) == -1) {
        handle_error("Close failed: %s\n");
    }

    if (pid > 0) {
        shm_unlink("/pipeautique3");
    }
    exit(EXIT_SUCCESS);
}

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(char *message)
{
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}
