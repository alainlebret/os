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
 * @author Alain Lebret
 * @version	1.0
 * @date 2012-04-10
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * A simple test that creates a stream socket and gives it a name.
 */
int main(int argc, char **argv) {
	int sd, port = 6543;
	struct sockaddr_in name;
	struct hostent *hostinfo;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s hostname\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Create the socket */
   sd = socket(PF_INET, SOCK_STREAM, 0);
   if (sd < 0) {
		fprintf(stderr, "socket() failed\n");
      exit(EXIT_FAILURE);
   }

	/* Give a name to the socket */
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	hostinfo = gethostbyname(argv[1]);
	if (hostinfo == NULL) {
		fprintf(stderr, "Unknown host %s.\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	name.sin_addr = *(struct in_addr *) hostinfo->h_addr;

   if (bind(sd, (struct sockaddr *) &name, sizeof(name)) < 0) {
		fprintf(stderr, "bind() failed\n");
		exit(EXIT_FAILURE);
   }

   exit(EXIT_SUCCESS);
}
