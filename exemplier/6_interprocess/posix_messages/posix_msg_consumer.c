/* Recepteur avec file de messages POSIX */
/* http://www.blaess.fr/christophe/ */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define __MAC_OS_X
#ifdef __MAC_OS_X
#else
#include <mqueue.h>
#endif

#define EVER ;;

int main(int argc, char *argv[]) {
#ifndef __MAC_OS_X
	mqd_t mq;
#endif
	size_t taille;
	char *buffer;
	struct mq_attr attr;
	struct timeval heure;
	struct timeval *recue;

	int nb_messages;
	long int duree;
	long int duree_max;
	long int duree_min;
	long int somme_durees;

	if (argc != 2) {
		fprintf(stderr, "usage: %s nom_file_message\n", argv[0]);
		exit(EXIT_FAILURE);
	}

#ifndef __MAC_OS_X
	mq = mq_open(argv[1], O_RDONLY | O_CREAT, 0600, NULL);
	if (mq == (mqd_t) - 1) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}

	if (mq_getattr(mq, &attr) != 0) {
		perror("mq_getattr");
		exit(EXIT_FAILURE);
	}
#endif

	taille = attr.mq_msgsize;
	buffer = malloc(taille);
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	recue = (struct timeval *) buffer;

	for (EVER) {
		nb_messages = 0;
		duree_max = 0;
		duree_min = -1;
		somme_durees = 0;

		do {
#ifndef __MAC_OS_X
			mq_receive(mq, buffer, taille, NULL);
#endif
			gettimeofday(&heure, NULL);
			duree = heure.tv_sec - recue->tv_sec;
			duree *= 1000000;
			duree += heure.tv_usec - recue->tv_usec;
			if (nb_messages > 0) { /* Ignorer le premier message (retarde) */
				if (duree_max < duree)
					duree_max = duree;
				if ((duree_min == -1) || (duree_min > duree))
					duree_min = duree;
				somme_durees += duree;
			}
			nb_messages++;
		} while (nb_messages < 100000); /* arbitraire, de l'ordre de la seconde */

		fprintf(stdout, "min =%3ld   max =%3ld moy=%5.1f\n",
				  duree_min, duree_max, ((float) somme_durees) / (nb_messages - 1));
	}

	exit(EXIT_SUCCESS); /* unreachable code */
}
