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

/* for inet_ntoa() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>

/**
 * @file testgethostbyname.c
 *
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

/**
 * A simple test of the gethostbyname function.
 */
int main(int argc, char **argv)
{
    struct hostent *hp;
    unsigned int count;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    hp = gethostbyname(argv[1]);
    if (hp == NULL) {
        fprintf(stderr, "gethostbyname() failed\n");
        exit(EXIT_FAILURE);
    } else {
        printf("%s = ", hp->h_name);
        count = 0;

        while (hp->h_addr_list[count] != NULL) {
            printf("%s ", inet_ntoa(*(struct in_addr *) (hp->h_addr_list[count])));
            count++;
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}
