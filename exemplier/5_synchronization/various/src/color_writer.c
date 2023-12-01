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
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

/**
 * @file color_writer.c
 *
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2023-09-07
 */

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 1024
#define SEM_NAME "/my_semaphore"

/* Generate a random integer between min and max */
int random_int(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

int main() 
{
    int shm_fd;
    int red;
    int green;
    int blue;
    void* shm_ptr;
    char* color_data;
    sem_t* semaphore;

    /* Create or open the shared memory segment */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    /* Set the size of the shared memory segment */
    ftruncate(shm_fd, SHM_SIZE);

    /* Map the shared memory into the address space */
    shm_ptr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    /* Pointer to the color data in shared memory */
    color_data = (char*)shm_ptr;

    /* Create or open the semaphore */
    semaphore = sem_open(SEM_NAME, O_CREAT, 0666, 1); /* Initial value is 1 */
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    /* Seed the random number generator with the current time */
    srand(time(NULL));

    while (1) {
        /* Generate random RGB color values */
        red = random_int(0, 255);
        green = random_int(0, 255);
        blue = random_int(0, 255);

        /* Format the color data as a string (e.g., "255,0,0" for red) */
        snprintf(color_data, SHM_SIZE, "%d,%d,%d", red, green, blue);

        /* Print the generated color for debugging */
        printf("Generated Color: %s\n", color_data);

        /* Sleep or do some work */
        usleep(1000000); /* Sleep for 1 second */
    }

    /* Unmap and close the shared memory segment when done */
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    /* Close and unlink the semaphore when done */
    sem_close(semaphore);
    sem_unlink(SEM_NAME);

    return EXIT_SUCCESS;
}
