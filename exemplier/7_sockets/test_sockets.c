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
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* close() */
#include <sys/types.h>  
#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* IPPROTO_XX */

#define EVER ;;

/**
 * New handler of the SIGINT signal.
 * @param signal Number of the signal
 */
void handle(int signal) {
	printf("SIGINT signal received!\n");

	exit(EXIT_SUCCESS);
}

int main() {
	int s1, s2, s3, s4, s5, s6, s7, s8; /* socket descriptors for the test */
	struct sigaction action;

	action.sa_handler = &handle;

	/* install the new handler of the SIGINT signal */
	sigaction(SIGINT, &action, NULL);

	/* open sockets in different modes */
	s1 = socket(AF_UNIX, SOCK_STREAM, 0);
	if (s1 == -1) {
		printf("Error opening a stream Unix socket using default protocol\n");
	} else {
		printf("A stream Unix socket (s1) using default protocol has been opened\n");
	}

	s2 = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (s2 == -1) {
		printf("Error opening a datagram Unix socket using default protocol\n");
	} else {
		printf("A datagram Unix socket (s2) using default protocol has been opened\n");
	}

	s3 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s3 == -1) {
		printf("Error opening a stream socket using TCP\n");
	} else {
		printf("A stream socket (s3) using TCP has been opened\n");
	}

	s4 = socket(AF_INET, SOCK_STREAM, 0);
	if (s4 == -1) {
		printf("Error opening a stream socket using default protocol\n");
	} else {
		printf("A stream socket (s4) using default protocol has been opened\n");
	}

	s5 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s5 == -1) {
		printf("Error opening a datagram socket using UDP\n");
	} else {
		printf("A datagram socket (s5) using UDP has been opened\n");
	}

	s6 = socket(AF_INET, SOCK_DGRAM, 0);
	if (s6 == -1) {
		printf("Error opening a datagram socket using default protocol\n");
	} else {
		printf("A datagram socket (s6) using default protocol has been opened\n");
	}

	s7 = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	if (s7 == -1) {
		printf("Error opening a raw socket using IP\n");
	} else {
		printf("A raw socket (s7) using IP has been opened\n");
	}

	s8 = socket(AF_INET, SOCK_RAW, 0);
	if (s8 == -1) {
		printf("Error opening a raw socket using default protocol\n");
	} else {
		printf("A raw socket (s8) using default protocol has been opened\n");
	}

	/* wait for Ctrl-C (SIGINT) */
	pause();

	close(s1);
	close(s2);
	close(s3);
	close(s4);
	close(s5);
	close(s6);
	close(s7);
	close(s8);

	exit(EXIT_SUCCESS);
}  
 