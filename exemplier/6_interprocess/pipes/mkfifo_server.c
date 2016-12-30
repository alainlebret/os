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
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file mkfifo_server.c
 *
 * A server program that returns the result of a a+b request sent by a client.
 * They both communicate using fifos (cli2serv and serv2cli).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define QUESTION "cli2serv"
#define REPONSE  "serv2cli"
#define EVER ;;

void traiter();  /* traitement par le serveur */

int main(void) {
	int fdq, fdr;

	unlink(QUESTION);
	unlink(REPONSE);

	/* Création des tubes nommés */
	if (mkfifo(QUESTION, 0644) == -1 || mkfifo(REPONSE, 0644) == -1) {
		perror("Impossible de créer les tubes");
		exit(EXIT_FAILURE);
	}

	/* Attente des ouvertures Clients */
	fdq = open(QUESTION, O_RDONLY);
	if (fdq == -1) {
		fprintf(stderr, "Impossible d\'ouvrir le tube %s\n", QUESTION);
		exit(EXIT_FAILURE);
	}

	fdr = open(REPONSE, O_WRONLY);
	if (fdr == -1) {
		fprintf(stderr, "Impossible d\'ouvrir le tube %s\n", REPONSE);
		exit(EXIT_FAILURE);
	}

	traiter(fdr, fdq);

	close(fdq);
	close(fdr);

	unlink(QUESTION);
	unlink(REPONSE);

	exit(EXIT_SUCCESS);
}

/**
 * Traitement par le serveur. Envoi de la réponse suite à la question "a opb"
 * en provenance du client (op vaut +, -, * ou /).
 * Arrêt dès la réception de la chaîne de caractères "Pouce!"
 * @param fdr descripteur du tube pour la réponse
 * @param fdq descripteur du tube pour la question
 */
void traiter(int fdr, int fdq) {
	int operande1, operande2, resultat;
	char operateur;
	char question[11];
	char reponse[11];

	for (EVER) {
		read(fdq, question, 10);
		sscanf(question, "%d%1s%d", &operande1, &operateur, &operande2);
		if (strcmp(question, "Pouce!") == 0) {
			strcpy(reponse, "OK");
			write(fdr, reponse, 10);
			break;
		}
		switch(operateur) {
			case '+' : resultat = operande1 + operande2; break;
			case '-' : resultat = operande1 - operande2; break;
			case '*' : resultat = operande1 * operande2; break;
			case '/' : resultat = (operande2 != 0) ? operande1 / operande2 : 32767 ; break;
			default : return;
		}
		if (resultat == 32767) strcpy(reponse, "NaN");

		sprintf(reponse, "%d", resultat);
		write(fdr, reponse, 10);
	}
}
