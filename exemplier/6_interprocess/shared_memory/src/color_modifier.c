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
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define MAX_MATRIX_SIZE 800

/**
 * @file color_modifier.c
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2023-09-30
 */

int main(int argc, char *argv[]) {
    int shm_fd;
    int *ptr;
    int i, j;
	int MATRIX_SIZE;

	/* Vérifie si la taille de la matrice est passée en argument */
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <size_of_the_shared_memory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Convertit l'argument en un entier */
    MATRIX_SIZE = atoi(argv[1]);
	
	shm_unlink("/matrix");

    /* Ouvre un segment de mémoire partagée */
    shm_fd = shm_open("/matrix", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /* Définit la taille du segment de mémoire partagée */
    if (ftruncate(shm_fd, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    /* Mappe le segment de mémoire partagée dans l'espace d'adressage */
    ptr = mmap(0, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* Initialise le générateur de nombres aléatoires */
    srand(time(NULL));

    while (1) {
		/* Remplit la matrice avec des valeurs de couleur aléatoires */
		for (i = 0; i < MATRIX_SIZE; i++) {
		    for (j = 0; j < MATRIX_SIZE; j++) {
		        ptr[(i * MATRIX_SIZE + j) * 3] = rand() % 256; // Red
		        ptr[(i * MATRIX_SIZE + j) * 3 + 1] = rand() % 256; // Green
		        ptr[(i * MATRIX_SIZE + j) * 3 + 2] = rand() % 256; // Blue
		    }
		}
        /* Attends pendant un certain temps */
        usleep(500000); /* 500 ms */
    }

    /* Détache le segment de mémoire partagée */
    if (munmap(ptr, MATRIX_SIZE * MATRIX_SIZE * 3 * sizeof(int)) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    /* Ferme le descripteur de fichier de mémoire partagée */
    close(shm_fd);
	
	shm_unlink("/matrix");

    return EXIT_SUCCESS;
}
