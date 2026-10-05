/*
 * Unix System Programming Examples / Exemplier de programmation système Unix
 *
 * Copyright (C) 1995-2026 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
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
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * @file mkfifo_server.c
 *
 * A server program that returns the result of a a+b request sent by a client.
 * They both communicate using fifos.
 *
 * Simplification: each read() is taken as one whole request. A pipe is a
 * stream of bytes, so this holds only because requests are short and the
 * client waits for each response before sending the next one.
 */

#define QUESTION "cli2serv"
#define RESPONSE "serv2cli"
/* SECURITY NOTE: predictable FIFO names in shared directories are vulnerable to local attacks.
 * Production code should use private directories, strict permissions, and peer validation. */

/**
 * Reads the expression "a <op> b" (op = '+', '-', '*' or '/') from the
 * the named pipe of given descriptor \c fdq and calculates it. The response
 * is sent to the named pipe of given descriptor \c fdr. The server stops when
 * receiving the string "Pouce!".
 * @param fdr The descriptor of the named pipe for the response to the client.
 * @param fdq The descriptor of the named pipe to receive queries from the client.
 */
void manage_server(int fdr, int fdq);

int main(void) {
    int fdq;
    int fdr;

    /* If the client leaves, write() fails with EPIPE instead of killing the
       server with SIGPIPE: the server can then remove the FIFOs */
    signal(SIGPIPE, SIG_IGN);

    unlink(QUESTION);
    unlink(RESPONSE);

    /* Création des tubes nommés */
    if (mkfifo(QUESTION, 0644) == -1 || mkfifo(RESPONSE, 0644) == -1) {
        perror("Impossible de créer les tubes");
        exit(EXIT_FAILURE);
    }

    /* Attente des ouvertures Clients */
    fdq = open(QUESTION, O_RDONLY);
    if (fdq == -1) {
        fprintf(stderr, "Unable to access to input named pipe %s\n", QUESTION);
        exit(EXIT_FAILURE);
    }

    fdr = open(RESPONSE, O_WRONLY);
    if (fdr == -1) {
        fprintf(stderr, "Unable to access to output named pipe %s\n", RESPONSE);
        exit(EXIT_FAILURE);
    }

    manage_server(fdr, fdq);

    close(fdq);
    close(fdr);

    unlink(QUESTION);
    unlink(RESPONSE);

    return EXIT_SUCCESS;
}

void manage_server(int fdr, int fdq) {
    int operand1, operand2, result = 0;
    const char *error;
    char operator;
    char question[11];
    char response[11];
    ssize_t bytes_read, bytes_written;

    while (1) {
        bytes_read = read(fdq, question, 10);
        if (bytes_read == -1) {
            perror("Error reading from pipe");
            break;
        }
        if (bytes_read == 0) { /* EOF: the client has closed the pipe */
            break;
        }
        question[bytes_read] = '\0'; /* Null-terminate the question */

        if (strncmp(question, "Pouce!", 6) == 0) {
            snprintf(response, sizeof(response), "OK");
            if (write(fdr, response, strlen(response) + 1) == -1) { /* +1 for '\0' */
                perror("Error writing to pipe");
            }
            break;
        }

        /* SECURITY NOTE: parse the operator as a char to avoid writing a string
         * into one-byte storage. In production, also validate input length/range. */
        if (sscanf(question, "%d %c %d", &operand1, &operator, &operand2) < 3) {
            snprintf(response, sizeof(response), "Error");
        } else {
            error = NULL;
            switch (operator) {
                case '+' :
                    result = operand1 + operand2;
                    break;
                case '-' :
                    result = operand1 - operand2;
                    break;
                case '*' :
                    result = operand1 * operand2;
                    break;
                case '/' :
                    if (operand2 == 0) {
                        error = "NaN";
                    } else {
                        result = operand1 / operand2;
                    }
                    break;
                default :
                    /* No 'continue': the client waits for a response */
                    error = "Invalid Op";
                    break;
            }

            if (error != NULL) {
                snprintf(response, sizeof(response), "%s", error);
            } else {
                snprintf(response, sizeof(response), "%d", result);
            }
        }

        bytes_written = write(fdr, response, strlen(response) + 1);
        if (bytes_written == -1) {
            perror("Error writing to pipe");
            break;
        }
    }
}
