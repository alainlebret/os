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
 * @file mkfifo_client.c
 *
 * A client program that sends a request to a server to calculate a<op>b.
 * They both communicate using two fifos (cli2serv and serv2cli).
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
#define FOREVER for (;;)

void traiter(int fdr, int fdq);  /* traitement par le client */

int main(void)
{
    int fdq;
    int fdr;

    fdq = open(QUESTION, O_WRONLY);
    if (fdq == -1) {
        fprintf(stderr, "Impossible d\'ouvrir le tube %s\n", QUESTION);
        fprintf(stderr, "Lancer le serveur d\'abord\n");
        exit(EXIT_FAILURE);
    }

    fdr = open(REPONSE, O_RDONLY);
    if (fdr == -1) {
        fprintf(stderr, "Impossible d\'ouvrir le tube %s\n", REPONSE);
        fprintf(stderr, "Lancer le serveur d\'abord\n");
        exit(EXIT_FAILURE);
    }

    traiter(fdr, fdq);

    close(fdq);
    close(fdr);

    exit(EXIT_SUCCESS);
}

/**
 * traitement client : lecture expression "a<op>b" dans stdin et écriture de la
 * réponse dans stdout. Arrêt si reponse = "OK".
 * @param fdr descripteur du tube pour la réponse
 * @param fdq descripteur du tube pour la question
 */
void traiter(int fdr, int fdq)
{
    char reponse[11];
    char question[11];

    FOREVER {
        if (fgets(question, 11, stdin) == NULL) {
            exit(EXIT_FAILURE);
        }
        write(fdq, question, 10);
        printf("Client -> %s \n", question);

        read(fdr, reponse, 10);
        printf("Serveur -> %s \n", reponse);
        if (strcmp(reponse, "OK") == 0) {
            break;
        }
    }
}

