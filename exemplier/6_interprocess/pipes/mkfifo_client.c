/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2022 Alain Lebret (alain.lebret [at] ensicaen [dot] fr)
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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define QUESTION "cli2serv"
#define RESPONSE "serv2cli"
#define FOREVER for (;;)

/**
 * @author Alain Lebret <alain.lebret@ensicaen.fr>
 * @version	1.0
 * @date 2011-12-01
 */

/**
 * @file mkfifo_client.c
 *
 * A client program that sends a request to a server to calculate a <op> b.
 * They both communicate using two fifos (cli2serv and serv2cli).
 */

/**
 * Reads the expression "a <op> b" from stdin and sends it to the server
 * through the named pipe of given descriptor \c fdq. The response from the
 * server is read from the named pipe of given descriptor \c fdr and is
 * send to stdout. The function exit when receiving "OK".
 * @param fdr The descriptor of the named pipe for the response from the server.
 * @param fdq The descriptor of the named pipe to send queries to the server.
 */
void manage_client(int fdr, int fdq);

int main(void)
{
    int fdq;
    int fdr;

    fdq = open(QUESTION, O_WRONLY);
    if (fdq == -1) {
        fprintf(stderr, "Unable to access to input named pipe %s\n", QUESTION);
        fprintf(stderr, "Please, run server first!\n");
        exit(EXIT_FAILURE);
    }

    fdr = open(RESPONSE, O_RDONLY);
    if (fdr == -1) {
        fprintf(stderr, "Unable to access to output named pipe %s\n", RESPONSE);
        fprintf(stderr, "Please, run server first!\n");
        exit(EXIT_FAILURE);
    }

    manage_client(fdr, fdq);

    close(fdq);
    close(fdr);

    exit(EXIT_SUCCESS);
}

void manage_client(int fdr, int fdq)
{
    char response[11];
    char query[11];

    FOREVER {
        if (fgets(query, 11, stdin) == NULL) {
            exit(EXIT_FAILURE);
        }
        write(fdq, query, 10);
        printf("Client -> %s \n", query);

        read(fdr, response, 10);
        printf("Server -> %s \n", response);
        if (strcmp(response, "OK") == 0) {
            break;
        }
    }
}

