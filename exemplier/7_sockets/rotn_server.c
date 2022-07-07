/*
 * ENSICAEN
 * 6 Boulevard Maréchal Juin
 * F-14050 Caen Cedex
 *
 * Unix System Programming Examples / Exemplier de programmation système Unix
 * Chapter "Network communication" / Chapitre "Communication par sockets"
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
 * @file rotn_server.c
 *
 * A TCP server that listens for connections on port 6789. It reads data
 * from its input (one line at a time), and writes out the ROTn obfuscation
 * of lines. It uses the Unix fork() primitive to create a new process for
 * each incoming connection.
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> /* Internet structures and functions. */
#include <sys/socket.h> /* Socket functions. */

#define FOREVER for(;;)
#define MAX_LINE 16384

char rot_char(char c, int rot)
{
    char result = c;

    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        result += rot;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        result -= rot;

    return result;
}

void handle_child(int fd)
{
    char outbuf[MAX_LINE + 1];
    size_t outbuf_used = 0;
    ssize_t result;

    FOREVER {
        char ch;

        /* Read each character from the client */
        result = recv(fd, &ch, 1, 0);
        if (result == 0) {
            break;
        } else if (result == -1) {
            perror("read");
            break;
        }

        /* Test not to overflowing the output buffer. */
        if (outbuf_used < sizeof(outbuf)) {
            outbuf[outbuf_used++] = rot_char(ch, 3);
        }

        if (ch == '\n') {
            /* Send message to the socket of the incoming connection */
            send(fd, outbuf, outbuf_used, 0);
            outbuf_used = 0;
            continue;
        }
    }
}

void run(void)
{
    int listener;
    struct sockaddr_in sin;

    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
    sin.sin_family = AF_INET;
    /* Set IP address to localhost */
    sin.sin_addr.s_addr = 0;
    /* Set port number, using htons function to use proper byte order */
    sin.sin_port = htons(6789);

    /* Create the socket. The three arguments are: 1) Internet domain 2) Stream
     * socket 3) Default protocol (TCP in this case)
     */
    listener = socket(AF_INET, SOCK_STREAM, 0);

    /* Bind the address struct to the socket */
    if (bind(listener, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }

    /* Listen on the socket, with 10 max connection requests queued */
    if (listen(listener, 10) < 0) {
        perror("listen");
        return;
    }


    FOREVER {
        struct sockaddr_storage ss;
        socklen_t slen = sizeof(ss);

        /* Accept call creates a new socket for the incoming connection */
        int fd = accept(listener, (struct sockaddr *) &ss, &slen);
        if (fd < 0) {
            perror("accept");
        } else {
            if (fork() == 0) {
                handle_child(fd);
                exit(EXIT_SUCCESS);
            }
        }
    }
}

int main(int c, char **v)
{
    run();
    exit(EXIT_SUCCESS);
}
