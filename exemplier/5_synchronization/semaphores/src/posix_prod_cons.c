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
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

/**
 * @file posix_prod_cons.c
 *
 * Producer-consumer program using a POSIX semaphore.
 */

#define BUFFER_SIZE  5
#define MEM_PATH     "/theshm"
#define SEM_PATH     "/thesemaphore"

typedef sem_t semaphore_t;

void handle_fatal_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void display_buffer(int *buffer) {
    printf("Buffer State: [ ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("]\n");
}

semaphore_t *setup_semaphore(const char *name, int value) {
    semaphore_t *sem = sem_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, value);
    if (sem == SEM_FAILED) {
        handle_fatal_error("sem_open failed");
    }
    return sem;
}

int *setup_shared_memory(const char *name, size_t size) {
    int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        handle_fatal_error("shm_open failed");
    }
    if (ftruncate(fd, size) == -1) {
        handle_fatal_error("ftruncate failed");
    }
    int *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        handle_fatal_error("mmap failed");
    }
    close(fd);  /* Close file descriptor as it's no longer needed */
    return addr;
}

void clean_up(int signum) {
    sem_unlink(SEM_PATH);
    shm_unlink(MEM_PATH);
    exit(EXIT_SUCCESS);
}

void produce(int *buffer, semaphore_t *sem) {
    int in_index = 0;
    srand((unsigned int)getpid());
    while (1) {
        sleep(1 + rand() % 5);
        int value = rand() % 100;
        sem_wait(sem);
        buffer[in_index] = value;
        printf("Produced: %d at %d\n", value, in_index);
        display_buffer(buffer);
        in_index = (in_index + 1) % BUFFER_SIZE;
        sem_post(sem);
    }
}

void consume(int *buffer, semaphore_t *sem) {
    int out_index = 0;
    srand((unsigned int)getpid());
    while (1) {
        sem_wait(sem);
        int value = buffer[out_index];
        printf("Consumed: %d from %d\n", value, out_index);
        display_buffer(buffer);
        out_index = (out_index + 1) % BUFFER_SIZE;
        sem_post(sem);
        sleep(2 + rand() % 5);
    }
}

int main(void) {
    struct sigaction sa;
    sa.sa_handler = clean_up;
    sigaction(SIGINT, &sa, NULL);

    semaphore_t *sem = setup_semaphore(SEM_PATH, 1);
    int *buffer = setup_shared_memory(MEM_PATH, BUFFER_SIZE * sizeof(int));

    pid_t pid = fork();
    if (pid == 0) {
        produce(buffer, sem);
        exit(EXIT_SUCCESS);
    } else {
        pid = fork();
        if (pid == 0) {
            consume(buffer, sem);
            exit(EXIT_SUCCESS);
        }
    }

    wait(NULL);
    wait(NULL);

    return EXIT_SUCCESS;
}
