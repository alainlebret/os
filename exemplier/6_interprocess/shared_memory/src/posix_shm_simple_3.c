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
#include <time.h>
#include <errno.h>

/**
 * @file posix_shm_simple_3.c
 *
 * Another example using parent and child processes sharing a complex structure memory.
 * Link with \c -lrt.
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2022-05-23
 */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_error(const char *message);

#define PAGESIZE 4096

typedef struct vector {
    float x;
    float y;
    float z;
} vector_t;

typedef struct color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} color_t;

typedef struct s1 {
    int id;
    char name[20];
    int age;
    char padding[PAGESIZE - 2 * sizeof(int) - 20 * sizeof(char)];
} s1_t;

typedef struct s2 {
    vector_t vec;
    color_t col;
    char padding[PAGESIZE - sizeof(vector_t) - 20 * sizeof(color_t)];
} s2_t;

typedef struct s1_et_s2 {
    s1_t a_s1;
    s2_t a_s2;
} s1_and_s2_t;


int main(void)
{
    int shm_fd;
    pid_t pid;
    s1_t *ptr1;
    s2_t *ptr2;

    srand(time(NULL));

    /* create the shared memory segment as if it was a file */
    shm_fd = shm_open("/pipeautique3", O_CREAT | O_RDWR, 0644);
    if (shm_fd == -1) {
        handle_error("Error [shm_open()]: ");
    }

    ftruncate(shm_fd, sizeof(s1_and_s2_t));
	if (ftruncate(shm_fd, sizeof(s1_and_s2_t)) == -1) {
	    handle_error("Error setting size with ftruncate: ");
	}
	
    /* map the shared memory segment for struct s1 to the address space of the process */
    ptr1 = (s1_t *) mmap(NULL, sizeof(s1_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr1 == MAP_FAILED) {
        handle_error("Error [mmap() ptr1]: ");
    }

    /* map the shared memory segment for struct s2 to the address space of the process */
    ptr2 = (s2_t *) mmap(NULL, sizeof(s2_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, sizeof(s1_t));
    if (ptr2 == MAP_FAILED) {
        handle_error("Error [mmap() ptr2]: ");
    }

    printf("> %p\n", (void *) ptr1);
    printf("> %p\n", (void *) ptr2);

    pid = fork();
    if (pid < 0) {
        handle_error("Error [fork()]: ");
    }
    if (pid > 0) { /* parent process */
	    strncpy(ptr1->name, "Monkeypox", 20);
        ptr2->col.red = 112;
    } else { /* child process */
        printf("%s\n", ptr1->name);
        printf("%i\n", ptr2->col.red);
    }

    /* remove the mapped memory segments from the address space of the process */
    if (munmap(ptr1, sizeof(s1_t)) == -1) {
        handle_error("Error [munmap() ptr1]: ");
    }
    if (munmap(ptr2, sizeof(s2_t)) == -1) {
        handle_error("Error [munmap() ptr2]: ");
    }

    /* close the shared memory segment as if it was a file */
    if (close(shm_fd) == -1) {
        handle_error("Error [close()]: ");
    }

    if (pid > 0) {
        shm_unlink("/pipeautique3");
    }
    
	return EXIT_SUCCESS;
}

void handle_error(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}
