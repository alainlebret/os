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
 * This work is licensed under a Creative Commons Attribution-ShareAlike 4.0
 * International License. You should have received a copy of the License.
 * If not, see https://creativecommons.org/licenses/by-sa/4.0/.
 */

/**
 * @author Alain Lebret
 * @version	1.0
 * @date 2016-11-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define EVER ;;
#define CHEMIN_MEMOIRE "/memoire"

struct memoire_t {
	int valeur;
	double racine;
};

void traiter_erreur(char *message) {
	perror(message);
	exit(EXIT_FAILURE);
}

void traiter_sigint(int signum) {
	if (shm_unlink("CHEMIN_MEMOIRE") < 0) {
		traiter_erreur("Erreur lors de l'appel à shm_unlink\n");
	}
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {
	int descripteur_memoire;
	int entier;
	size_t taille_memoire = (1 * sizeof(struct memoire_t));
	struct memoire_t *memoire;

	signal(SIGINT, traiter_sigint);

	descripteur_memoire = shm_open(CHEMIN_MEMOIRE, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
	if (descripteur_memoire < 0) {
		traiter_erreur("Erreur lors de l'appel à shm_open\n");
	}

	fprintf(stderr, "Objet de mémoire partagée %s créé\n", CHEMIN_MEMOIRE);

	ftruncate(descripteur_memoire, taille_memoire);

	memoire = (struct memoire_t *) mmap(NULL,
													taille_memoire,
													PROT_READ | PROT_WRITE,
													MAP_SHARED,
													descripteur_memoire,
													0);
	if (memoire == MAP_FAILED) {
		traiter_erreur("Erreur lors de l'appel à mmap\n");;
	}
	fprintf(stderr, "Mémoire de %d octets correctement allouée.\n", taille_memoire);

	entier = 1;
	for (EVER) {
		memoire->valeur = entier;
		memoire->racine = sqrt(entier);
		sleep(5);
		entier++;
	}

	/* unreachable code */
	if (shm_unlink(CHEMIN_MEMOIRE) != 0) {
		traiter_erreur("Erreur lors de l'appel à shm_unlink\n");
	}

	exit(EXIT_SUCCESS);
}